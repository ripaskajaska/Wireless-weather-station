import serial
import platform
import serial.tools
import serial.tools.list_ports
import re
import requests
import datetime
import os
import csv

curr_platform = platform.system()

api_password = "" #This is not public
desired_pid = '0x6015'
desired_vid = '0x0403'
desired_sn = 'D3OHLEJRA'

temp_regex = r"Temperature:\s*([\d.]+)°C"
press_regex = r"Pressure:\s*([\d.]+)\s*hPa"
hum_regex = r"Humidity:\s*([\d.]+)%"
vol_regex = r"Battery\s*voltage:\s*([\d.]+)\s*V"

compiled_temp_reg = re.compile(temp_regex)
compiled_pres_reg = re.compile(press_regex)
compiled_hum_reg = re.compile(hum_regex)
compiled_vol_reg = re.compile(vol_regex)


def find_serial_ports():
    port_list = []

    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        if port.pid == desired_pid or port.vid == desired_vid or port.serial_number == desired_sn:
            port_list.append(port.device)
            print(f"Found port: {port.device} (Description: {port.description})")
            return port_list

    if len(port_list) == 0 and curr_platform == 'Darwin':
        for port in ports:
            if 'usb' in port.device.lower():
                port_list.append(port.device)
    else:
        for port in ports:
            port_list.append(port.device)
    return port_list


def create_serial_object(port=None, baudrate=None):
    serial_obj = serial.Serial(port=port, baudrate=baudrate, timeout=1)
    return serial_obj


def parse_lines(temp_line, pres_line, hum_line, vol_line):
    try:
        temp_match = re.match(compiled_temp_reg, temp_line)
        pres_match = re.match(compiled_pres_reg, pres_line)
        hum_match = re.match(compiled_hum_reg, hum_line)
        vol_match = re.match(compiled_vol_reg, vol_line)
        temp_val = float(temp_match.group(1))
        pres_val = float(pres_match.group(1))
        hum_val = float(hum_match.group(1))
        vol_val = float(vol_match.group(1))
        return temp_val, pres_val, hum_val, vol_val
    except TypeError as e:
        print(e)
        return None, None, None, None
    except ValueError as e:
        print(e)
        return None, None, None, None

def main():
    obj = None
    print("Welcome to the Weather Station serial data reader!")
    print(f"Current platform: {curr_platform}")
    try:
        found_ports = find_serial_ports()
        if len(found_ports) == 1:
            port = found_ports[0]
        elif len(found_ports) > 1:
            print("Found the following ports:")
            for port in found_ports:
                print(port)
            print("Choose one of these...")
            while True:
                chosen_port = input("Port to be used: ")
                if len(chosen_port) in found_ports:
                    port = chosen_port
                    break
                else:
                    print("Didn't quite catch that. Try again")

        else:
            print("No ports found")

        baudrate = 9600
        obj = create_serial_object(port, baudrate=baudrate)
        print("Serial object initialized. Press Ctrl+C to stop.")
        if not os.path.exists("weather_data.csv"):
            with open("weather_data.csv", "w", newline='') as csv_file:
                row_writer = csv.writer(csv_file, delimiter=',')
                row_writer.writerow(['time', 'temperature', 'pressure', 'humidity', 'voltage'])

        while True:
            with open("weather_data.csv", "a", newline='') as csv_file:
                row_writer = csv.writer(csv_file, delimiter=',')
                if obj.in_waiting > 0:
                    print("Serial printing")
                    read_bytes = obj.readline()
                    temp_line = read_bytes.decode(encoding="utf-8", errors="ignore")
                    temp_line = temp_line.rstrip()
                    if temp_line.startswith("Temp"):
                        pres_line = obj.readline().decode(encoding="utf-8", errors="ignore")
                        temp_line = temp_line.rstrip()
                        hum_line = obj.readline().decode(encoding="utf-8", errors="ignore")
                        temp_line = temp_line.rstrip()
                        vol_line = obj.readline().decode(encoding="utf-8", errors="ignore")
                        temp_line = temp_line.rstrip()
                        temp, pres, hum, vol = parse_lines(temp_line, pres_line, hum_line, vol_line)
                        val_list = [temp, pres, hum, vol]
                        if all(isinstance(i, float) for i in val_list):
                            round_temp = round(temp, 2)
                            round_pres = round(pres / 1000, 2)
                            round_hum = round(hum, 2)
                            round_vol = round(vol, 2)
                            data = {
                                'temperature': round_temp,
                                'pressure': round_pres,
                                'humidity': round_hum,
                                'voltage': round_vol,
                                'password': api_password,
                            }
                            print(f"\nReceived temperature: {temp}°C")
                            print(f"\nReceived pressure: {pres} Pa")
                            print(f"\nReceived humidity: {hum}%")
                            print(f"\nReceived voltage: {vol}V")
                            response: requests.Response = requests.api.post(
                                url="https://weather-station.deno.dev/api/weather", json=data)
                            if response.ok:
                                print("Successfully sent data to api")
                                row_writer.writerow([datetime.datetime.utcnow(), round_temp, round_pres, round_hum, round_vol])
                            else:
                                print("Data did not go through")
                                reason = response.reason
                                if reason:
                                    print(f"Reason: {reason}")
    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("\nTerminated by user.")
    except OSError:
        print("Device connection issues or you opened data file while executing.\nTo prevent the execution from failing, please make a copy of the data file and open that instead.")
    finally:
        if obj.is_open:
            obj.close()
            print("Serial connection closed.")
        


if __name__ == "__main__":
    main()
