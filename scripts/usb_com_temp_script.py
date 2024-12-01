import serial
import platform
import serial.tools
import serial.tools.list_ports
import re
import requests
import struct
import math
curr_platform = platform.system()

api_password = "peij00niSetit"
desired_pid = '0x6015'
desired_vid = '0x0403'
desired_sn = 'D3OHLEJRA'

regex = r"Measured\s*temperature:\s*([\d.]+)°C"
compiled_reg = re.compile(regex)

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


def create_serial_object(port = None, baudrate= None):
    serial_obj = serial.Serial(port=port, baudrate=baudrate, timeout=1)
    return serial_obj

def parse_temp_line(line: str):
    try:
        match = re.match(regex, line)
        value = float(match.group(1))
        return value
    except TypeError or ValueError:
        return False


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
        while True:
            if obj.in_waiting > 0:
                print("Serial printing")
                read_bytes = obj.read_until()
                float_val = parse_temp_line(read_bytes.decode(encoding="utf-8", errors="ignore"))
                if isinstance(float_val, float):
                    data = {
                        'temperature': round(float_val, 1),
                        'pressure': 0,
                        'humidity': 0,
                        'voltage': 0,
                        'password': api_password,
                    }
                    print(f"\nReceived temperature: {float_val}°C")
                    response: requests.Response = requests.api.post(url="https://weather-station.deno.dev/api/weather", json=data)
                    if response.ok:
                        print("Successfully sent data to api")
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
        print("Device connection issues")
    finally:
        if obj.is_open:
            obj.close()
            print("Serial connection closed.")
        quit(0)
    

if __name__ == "__main__":
    main()