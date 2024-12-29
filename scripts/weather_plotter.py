import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import datetime


def hourly_temperature():
    with open("/Users/rjaskari/Documents/Koulu/elepaja/Wireless-weather-station/weather_data.csv", "r") as csv_file:
        df = pd.read_csv(csv_file, names=['time', 'temperature', 'pressure', 'humidity', 'voltage'], header=0)
        df['time'] = pd.to_datetime(df['time'])
        df.set_index('time', inplace=True)

        hourly_min = df['temperature'].resample('h').min()
        hourly_max = df['temperature'].resample('h').max()
        hourly_avg = df['temperature'].resample('h').mean()

        hourly_temperatures = pd.DataFrame({
        'min': hourly_min,
        'max': hourly_max,
        'avg': hourly_avg
        })
        
        x = np.arange(len(hourly_temperatures))
        bar_width = 0.7

        plt.figure(figsize=(21, 10))
        plt.bar(x , hourly_temperatures['max'], bar_width, label='Max', alpha=0.7, color='orange')
        plt.bar(x, hourly_temperatures['min'], bar_width, label='Min', alpha=0.7, color='darkblue')
        plt.plot(x, hourly_temperatures['avg'], label='Avg', color='green', marker='o', linestyle='-', linewidth=2)
        plt.xticks(x, hourly_temperatures.index.strftime('%H:%M'), rotation=45)  
        plt.title('Hourly Temperature Statistics')
        plt.xlabel('Hour')
        plt.ylabel('Temperature (°C)')
        plt.legend()
        plt.tight_layout()
        plt.grid(axis='y', linestyle='--', alpha=0.5)
        plt.show()

def hourly_pressure_and_humidity():
    with open("/Users/rjaskari/Documents/Koulu/elepaja/Wireless-weather-station/weather_data.csv", "r") as csv_file:
        df = pd.read_csv(csv_file, names=['time', 'temperature', 'pressure', 'humidity', 'voltage'], header=0)
        df['time'] = pd.to_datetime(df['time'])
        df.set_index('time', inplace=True)

        hourly_avg_pres = df['pressure'].resample('h').mean()
        hourly_avg_hum = df['humidity'].resample('h').mean()
        x = np.arange(len(hourly_avg_pres))

        fig, ax = plt.subplots(figsize=(21, 10))
        plt.title('Hourly Pressure and Humidity')
        ax2 = ax.twinx()
        line1, = ax.plot(x, hourly_avg_pres, label='Average pressure', color='green', marker='o', linestyle='-', linewidth=2)
        line2, = ax2.plot(x, hourly_avg_hum, label='Average humidity', color='orange', marker='o', linestyle='-', linewidth=2)
        tick_positions = x  # All tick positions
        label_positions = x[::3]  # Show a label for every second tick
        labels = hourly_avg_pres.index.strftime('%H:%M')
        label_values = [label if i in label_positions else "" for i, label in enumerate(labels)]
        plt.xticks(tick_positions, label_values, rotation=45)             
        ax.set_xlabel('Hour')
        ax.set_ylabel("Pressure (hPa)")
        ax2.set_ylabel("Humidity (%)")
        lines = [line1, line2]  # Add all lines from ax and ax2
        labels = ['Average pressure', 'Average humidity']
        ax.legend(lines, labels)
        plt.tight_layout()
        plt.grid(axis='y', linestyle='--', alpha=0.5)
        plt.show()

def hourly_battery_voltage():
   with open("/Users/rjaskari/Documents/Koulu/elepaja/Wireless-weather-station/weather_data.csv", "r") as csv_file:
        df = pd.read_csv(csv_file, names=['time', 'temperature', 'pressure', 'humidity', 'voltage'], header=0)
        df['time'] = pd.to_datetime(df['time'])
        df.set_index('time', inplace=True)

        hourly_avg_vol = df['voltage'].resample('h').mean()
        x = np.arange(len(hourly_avg_vol))

        fig, ax = plt.subplots(figsize=(21, 10))
        plt.title('Hourly Average Voltage')
        line1, = ax.plot(x, hourly_avg_vol, label='Average Voltage', color='orange', marker='o', linestyle='-', linewidth=2)
        tick_positions = x  # All tick positions
        label_positions = x[::3]  # Show a label for every second tick
        labels = hourly_avg_vol.index.strftime('%H:%M')
        label_values = [label if i in label_positions else "" for i, label in enumerate(labels)]
        plt.xticks(tick_positions, label_values, rotation=45)             
        ax.set_xlabel('Hour')
        ax.set_ylabel("Voltage (V)")
        ax.legend()
        plt.tight_layout()
        plt.grid(axis='y', linestyle='--', alpha=0.5)
        plt.show() 

    
hourly_battery_voltage()
hourly_pressure_and_humidity()
hourly_temperature()

    
