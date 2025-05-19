import sys
import random
import os
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QLabel, QProgressBar, QMessageBox, QFrame, QLCDNumber
)
from PyQt6.QtCore import QTimer, Qt, QTime, pyqtSlot
from PyQt6.QtGui import QPixmap

# Finner filbanen
def get_resource_path(rel_path):
    try:
        base = sys._MEIPASS
    except Exception:
        base = os.path.abspath(".")
    return os.path.join(base, rel_path)

# En enkel digital klokke basert på QLCDNumber
class SimpleClock(QLCDNumber):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setSegmentStyle(QLCDNumber.SegmentStyle.Filled)
        self.setDigitCount(8)
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_time)
        self.timer.start(1000)
        self.update_time()
        self.setStyleSheet("color: #d4af37; background-color: transparent;")
        self.setFixedWidth(150)

    @pyqtSlot()
    def update_time(self):
        now = QTime.currentTime()
        t_str = now.toString("hh:mm:ss")
        if now.second() % 2 == 0:
            t_str = t_str.replace(":", " ")
        self.display(t_str)

# Kaffemaskin-logikk (kaffe er gratis)
class SimpleCoffeeMaker:
    def __init__(self):
        self.stock = {"water": 300, "milk": 200, "coffee": 100}
        self.recipes = {
            "espresso": {"ingredients": {"water": 50, "milk": 0, "coffee": 18}},
            "cappuccino": {"ingredients": {"water": 250, "milk": 100, "coffee": 24}}
        }

    def get_stock_report(self):
        return (f"Water: {self.stock['water']}ml\n"
                f"Milk: {self.stock['milk']}ml\n"
                f"Coffee: {self.stock['coffee']}g")

    def get_recipe(self, kind):
        if kind in self.recipes:
            ing = self.recipes[kind]["ingredients"]
            return (f"{kind.capitalize()} oppskrift:\n"
                    f"  Water: {ing['water']}ml\n"
                    f"  Milk: {ing['milk']}ml\n"
                    f"  Coffee: {ing['coffee']}g")
        else:
            return "Ukjent kaffetype."

    def can_make(self, kind):
        for item, needed in self.recipes[kind]["ingredients"].items():
            if self.stock[item] < needed:
                return False, item
        return True, ""

    def use_ingredients(self, kind):
        for item, needed in self.recipes[kind]["ingredients"].items():
            self.stock[item] -= needed

    def refill(self):
        self.stock["water"] = 300
        self.stock["milk"] = 200
        self.stock["coffee"] = 100

# Egendefinert dialog for rapport (bruker QMessageBox for enkelhet)
class ReportWin(QMessageBox):
    def __init__(self, text, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Rapport")
        self.setText(text)
        self.setStandardButtons(QMessageBox.StandardButton.Close)
        self.setStyleSheet("QLabel {font-size: 14px; font-family: 'Segoe UI', sans-serif; color: #d4af37;}")

# Hovedvinduet til kaffemaskinen
class CoffeeWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Kaffeknekt Kaffemaskin")
        self.setGeometry(100, 100, 600, 400)
        self.maker = SimpleCoffeeMaker()
        self.setup_ui()

    def setup_ui(self):
        central = QWidget()
        self.setCentralWidget(central)
        lay = QVBoxLayout(central)
        
        # Øverste rad med klokke og stopp-knapp (til høyre)
        top_row = QHBoxLayout()
        top_row.addStretch()
        self.clock = SimpleClock()
        top_row.addWidget(self.clock)
        self.stop_btn = QPushButton("Avslutt")
        self.stop_btn.setStyleSheet("""
            QPushButton {
                background-color: #e74c3c;
                color: white;
                border: none;
                border-radius: 8px;
                padding: 5px 10px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover { background-color: #c0392b; }
        """)
        self.stop_btn.clicked.connect(self.stop_process)
        top_row.addWidget(self.stop_btn)
        lay.addLayout(top_row)
        
        # Logo
        self.logo_lbl = QLabel()
        pix = QPixmap(get_resource_path("LOGO.png"))
        if not pix.isNull():
            self.logo_lbl.setPixmap(pix.scaled(170, 170, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation))
        else:
            self.logo_lbl.setText("LOGO.png not found")
        self.logo_lbl.setAlignment(Qt.AlignmentFlag.AlignCenter)
        lay.addWidget(self.logo_lbl)
        
        # Hovedtittel
        self.title_lbl = QLabel("Please select your coffee")
        self.title_lbl.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.title_lbl.setStyleSheet("font-size: 24px; font-weight: bold;")
        lay.addWidget(self.title_lbl)
        
        # Kaffeknapper
        btn_style = """
            QPushButton {
                background-color: #3498db;
                color: white;
                border: none;
                border-radius: 10px;
                padding: 10px 20px;
                font-size: 16px;
            }
            QPushButton:hover { background-color: #2980b9; }
            QPushButton:pressed { background-color: #1f6391; padding-left: 15px; }
        """
        btn_row = QHBoxLayout()
        self.btn_esp = QPushButton("Espresso")
        self.btn_cap = QPushButton("Cappuccino")
        btn_row.addWidget(self.btn_esp)
        btn_row.addWidget(self.btn_cap)
        lay.addLayout(btn_row)
        self.btn_esp.setStyleSheet(btn_style)
        self.btn_cap.setStyleSheet(btn_style)
        self.btn_esp.clicked.connect(lambda: self.start_coffee("espresso"))
        self.btn_cap.clicked.connect(lambda: self.start_coffee("cappuccino"))
        
        # Rapport og refill
        ctrl_row = QHBoxLayout()
        self.btn_report = QPushButton("Vis rapport")
        self.btn_refill = QPushButton("Refill Maskin")
        ctrl_row.addWidget(self.btn_report)
        ctrl_row.addWidget(self.btn_refill)
        lay.addLayout(ctrl_row)
        self.btn_report.clicked.connect(self.show_report)
        self.btn_refill.clicked.connect(self.refill_maker)
        
        # Fremdriftslinje
        self.prog = QProgressBar()
        self.prog.setValue(0)
        lay.addWidget(self.prog)
        
        # Sensor-delen i en ramme
        sensor_frame = QFrame()
        sensor_frame.setStyleSheet("""
            QFrame {
                background-color: #3a3a3a;
                border: 2px solid #d4af37;
                border-radius: 10px;
                padding: 5px;
            }
            QLabel {
                color: #d4af37;
                font-size: 16px;
                font-family: 'Segoe UI', sans-serif;
                font-weight: bold;
            }
        """)
        sensor_lay = QVBoxLayout(sensor_frame)
        self.sensor_lbl = QLabel("Sensor Data:")
        self.sensor_lbl.setTextFormat(Qt.TextFormat.RichText)
        sensor_lay.addWidget(self.sensor_lbl)
        lay.addWidget(sensor_frame)
        
        # Timere
        self.timer_brew = QTimer()
        self.timer_brew.timeout.connect(self.update_progress)
        self.timer_sensor = QTimer()
        self.timer_sensor.timeout.connect(self.update_sensor)
        
        self.is_brewing = False
        self.curr_type = None

    def start_coffee(self, kind):
        if self.is_brewing:
            return
        can_make, lacking = self.maker.can_make(kind)
        if not can_make:
            QMessageBox.warning(self, "Feil", f"Ikke nok {lacking} for {kind}.")
            return
        self.curr_type = kind
        self.prog.setValue(0)
        self.is_brewing = True
        self.disable_all()
        self.title_lbl.setText(f"Lager {kind.capitalize()}...")
        self.timer_brew.start(1100)
        self.timer_sensor.start(500)

    def update_progress(self):
        val = self.prog.value()
        if val < 100:
            self.prog.setValue(val + 5)
        else:
            self.timer_brew.stop()
            self.timer_sensor.stop()
            self.finish_coffee()

    def update_sensor(self):
        temp = random.uniform(90, 96)
        pres = random.uniform(8.0, 10.0)
        text = f"<b>Temperatur:</b> {temp:.1f}°C<br><b>Trykk:</b> {pres:.2f} bar"
        self.sensor_lbl.setText(text)

    def finish_coffee(self):
        self.maker.use_ingredients(self.curr_type)
        QMessageBox.information(self, "Ferdig", f"Din {self.curr_type} er klar! Nyt gratis kaffe ☕️")
        self.is_brewing = False
        self.enable_all()
        self.curr_type = None
        self.prog.setValue(0)
        self.title_lbl.setText("Please select your coffee")

    def stop_process(self):
        if self.is_brewing:
            self.timer_brew.stop()
            self.timer_sensor.stop()
            self.is_brewing = False
            self.prog.setValue(0)
            self.title_lbl.setText("Brewing stopped")
            self.enable_all()

    def disable_all(self):
        self.btn_esp.setEnabled(False)
        self.btn_cap.setEnabled(False)
        self.btn_report.setEnabled(False)
        self.btn_refill.setEnabled(False)

    def enable_all(self):
        self.btn_esp.setEnabled(True)
        self.btn_cap.setEnabled(True)
        self.btn_report.setEnabled(True)
        self.btn_refill.setEnabled(True)

    def show_report(self):
        rcp = self.maker.get_recipe("espresso")
        stk = self.maker.get_stock_report()
        full = f"<pre>{rcp}\n\n{stk}</pre>"
        rep_win = ReportWin(full, self)
        rep_win.exec()

    def refill_maker(self):
        self.maker.refill()
        QMessageBox.information(self, "Refill", "Maskinen er fylt opp igjen!")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setStyleSheet("""
    QMainWindow { background-color: #2b2b2b; }
    QLabel { color: #d4af37; font-family: 'Segoe UI', sans-serif; }
    QPushButton {
        background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #3a3a3a, stop:1 #5a5a5a);
        color: #d4af37; border: 2px solid #d4af37;
        border-radius: 12px; padding: 8px 16px; font-size: 16px; font-weight: bold;
    }
    QPushButton:hover { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #5a5a5a, stop:1 #7a7a7a); }
    QPushButton:pressed { background-color: #3a3a3a; border: 2px solid #b08b3a; }
    QProgressBar {
        background-color: #3a3a3a; color: #d4af37;
        border: 2px solid #d4af37; border-radius: 10px; text-align: center;
    }
    QProgressBar::chunk {
        background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #d4af37, stop:1 #ffcc00);
        border-radius: 8px;
    }
    """)
    win = CoffeeWindow()
    win.show()
    sys.exit(app.exec())
