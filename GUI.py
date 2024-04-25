import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QComboBox, QLineEdit, QLabel
import serial

class SerialCommunication(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Serial Communication')
        self.setGeometry(100, 100, 400, 200)

        layout = QVBoxLayout()

        # ComboBox for selecting COM port
        self.comboCOM = QComboBox()
        self.comboCOM.addItems(['COM1', 'COM2', 'COM3'])
        layout.addWidget(self.comboCOM)

        # ComboBox for selecting Waveform
        self.comboWaveform = QComboBox()
        self.comboWaveform.addItems(['Square Wave', 'Staircase Wave', 'Sine Wave', 'Triangular Wave'])
        layout.addWidget(self.comboWaveform)

        self.address_input = QLineEdit()
        self.address_input.setPlaceholderText('Enter Amplitude of the signal')
        layout.addWidget(self.address_input)

        self.data_input = QLineEdit()
        self.data_input.setPlaceholderText('Enter Frequency of the signal')
        layout.addWidget(self.data_input)

        self.send_button = QPushButton('Send Data')
        self.send_button.clicked.connect(self.send_data)
        layout.addWidget(self.send_button)

        self.status_label = QLabel()
        layout.addWidget(self.status_label)

        self.setLayout(layout)

    def send_data(self):
        port = self.comboCOM.currentText()
        waveform = self.comboWaveform.currentIndex() + 1  # plus 1 because indices start at 0
        decimal_address = self.address_input.text()
        decimal_data = self.data_input.text()
        self.send_serial_data(port, waveform, decimal_address, decimal_data)

    def send_serial_data(self, port, waveform, decimal_address, decimal_data):
        try:
            address_dec = int(decimal_address)
            address_hex = hex(address_dec)[2:].zfill(2)  # Convert address to hexadecimal string
            data_dec = int(decimal_data)  # Convert data to integer
            data_hex = hex(data_dec)[2:].zfill(2)  # Convert data to hexadecimal string
        except ValueError:
            self.status_label.setText('Invalid input. Please enter valid decimal address and data.')
            return

        try:
            ser = serial.Serial(port, 9600, timeout=1)
            # ser.write(b'@')  # Start flag
            # ser.write(b'R')  # 'R' for read
            ser.write(bytes([waveform]))  # Send waveform as a byte
            ser.write(bytes.fromhex(address_hex))  # Convert hexadecimal address to bytes and send
            ser.write(bytes.fromhex(data_hex))  # Convert hexadecimal data to bytes and send
            # ser.write(b';')  # End flag
            ser.close()
            self.status_label.setText(f"Data '{data_hex}' sent successfully to register {address_hex} with waveform {waveform} on {port}")
        except Exception as e:
            self.status_label.setText(f"Error: {e}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = SerialCommunication()
    window.show()
    sys.exit(app.exec_())
