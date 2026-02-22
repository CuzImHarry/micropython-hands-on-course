import math
import time
from machine import Pin, SPI
import BGT60TRXX as BGT

class RadarController:
    def __init__(self, start_freq=57_500_000, bandwidth=5_000_000, adc_div=60, samples_per_chirp=128, no_of_chirps=1):
        self.no_of_chirps = no_of_chirps
        self.samples_per_chirp = samples_per_chirp
        self.words = samples_per_chirp * no_of_chirps
        self.adc_div = adc_div
        self.start_freq = start_freq
        self.bandwidth = bandwidth

        # Initialize SPI
        self.spi = SPI(
            baudrate=50_000_000, 
            polarity=0, 
            phase=0, 
            bits=8, 
            firstbit=SPI.MSB, 
            sck='P12_2', 
            mosi='P12_0', 
            miso='P12_1'
        )

        # Initialize Sensor Object
        self.sensor = BGT.BGT60TRxxModule(self.words, self.spi, Pin("P12_3"), Pin("P11_1"), Pin("P11_0"))
        
        # Configure Timing and Frequencies
        self.sensor.set_adc_div(self.adc_div)
        self.sensor.set_chirp_len(self.samples_per_chirp)
        
        fsu = BGT.calculateFSU(self.start_freq)
        rtu = BGT.calculateRTU(self.adc_div, self.samples_per_chirp)
        rsu = BGT.calculateRSU(self.bandwidth, rtu)
        
        self.sensor.configure_chirp(fsu, rtu, rsu)
        self.sensor.set_vga_gain_ch1(3)

    def setup(self):
        """Called in boot.py to prepare the sensor for data capture."""
        self.sensor.reset()
        self.sensor.initSensor()
        print("Radar sensor initialized successfully.")

    def read_fifo(self):
        """Triggers a frame, reads the raw buffer, and resets the FIFO."""
        self.sensor.startFrame()
        time.sleep_ms(5) # Allow time to capture
        
        try:
            # Note: adjust this method name if your specific wrapper differs
            raw_data = self.sensor.get_fifo_data() 
        except AttributeError:
            raw_data = []
            print("Error: Could not read FIFO. Check wrapper method name.")
            
        self.sensor.resetFIFO()
        return raw_data

# --- Math Utilities ---

def compute_fft_1d(x):
    """
    Pure Python recursive Cooley-Tukey 1D FFT.
    Expects x to be a list of numbers with a power-of-2 length (e.g., 128).
    """
    N = len(x)
    if N <= 1:
        return x
    
    # Split into even and odd indices
    even = compute_fft_1d(x[0::2])
    odd = compute_fft_1d(x[1::2])
    
    # Compute twiddle factors
    T = [complex(math.cos(-2 * math.pi * k / N), math.sin(-2 * math.pi * k / N)) for k in range(N // 2)]
    
    # Combine
    return [even[k] + T[k] * odd[k] for k in range(N // 2)] + \
           [even[k] - T[k] * odd[k] for k in range(N // 2)]

def compute_frobenius_norm(data):
    """
    Calculates the Frobenius norm of a 1D list or 2D list-of-lists.
    """
    sum_sq = 0.0
    
    # Check if data is 2D (list of lists)
    if isinstance(data[0], (list, tuple)):
        for row in data:
            for val in row:
                if isinstance(val, complex):
                    sum_sq += (val.real**2 + val.imag**2)
                else:
                    sum_sq += val**2
    else:
        # Data is 1D
        for val in data:
            if isinstance(val, complex):
                sum_sq += (val.real**2 + val.imag**2)
            else:
                sum_sq += val**2
                
    return math.sqrt(sum_sq)