import numpy as np
import matplotlib.pyplot as plt
import csv


def load_csv_data(filename, start_row=13):  #toi start_row on se mist data alkaa
    time_values = []
    measured_values = []

    with open(filename, 'r') as csvfile:
        reader = csv.reader(csvfile)

        # skippaa oikeeseen dataan asti
        for _ in range(start_row):
            next(reader)

        # lukee dataa
        for row in reader:
            if len(row) == 2:
                time_values.append(float(row[0]))
                measured_values.append(float(row[1]))

    return np.array(time_values), np.array(measured_values)


# plot time domain
def plot_time_domain(time, measured):
    plt.figure(figsize=(10, 6))
    plt.plot(time, measured, label='Measured Values')
    plt.xlabel('Time')
    plt.ylabel('Measured Values')
    plt.title('Time vs Measured Values')
    plt.grid(True)
    plt.legend()
    plt.show()


# calc. and plot FFT
def plot_fft(time, measured):
    N = len(time)  # sample amount
    T = np.mean(np.diff(time))  # Sampling interval

    # Perform FFT
    fft_values = np.fft.fft(measured)
    fft_freq = np.fft.fftfreq(N, T)

    # Only plot the positive frequencies
    idx = np.argsort(fft_freq)  # Sort frequencies
    fft_freq = fft_freq[idx][N // 2:]  # Keep positive frequencies
    fft_values = np.abs(fft_values[idx][N // 2:])  # Take absolute values of sorted FFT

    # Plot FFT
    plt.figure(figsize=(10, 6))
    plt.plot(fft_freq, fft_values, label='FFT of Measured Values')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Amplitude')
    plt.title('Frequency Domain (FFT)')
    plt.grid(True)
    plt.legend()
    plt.show()


if __name__ == "__main__":
    filename = '/Users/kallearo/Downloads/numpytest.csv'  # filepath to csv file
    time_values, measured_values = load_csv_data(filename)

    # Plot the time domain data
    plot_time_domain(time_values, measured_values)

    # Plot the FFT of the data
    plot_fft(time_values, measured_values)