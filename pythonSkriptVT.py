#!/usr/bin/env python3

import socket
import time
import random
import sys
import select

HOST = ''       # Lytt på alle nettverksgrensesnitt
PORT = 5000     # Velg en ledig port

def main():
    # 1. Sett opp TCP-server
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(1)
    print(f"[INFO] TCP-server venter på tilkobling på port {PORT}...")

    conn, addr = server_socket.accept()
    print(f"[INFO] Klient tilkoblet fra {addr}")

    # 2. Vent på "D"-kommando fra bruker for å generere bar-verdi
    #    Samtidig skal vi motta eventuelle forespørsler fra Qt, men i dette eksempelet
    #    bare kjører vi i en loop og sjekker tastaturet.
    while True:
        # a) Sjekk om vi har tastetrykk i terminalen (ikke strengt nødvendig, men du ba om "D" trykk)
        ready, _, _ = select.select([sys.stdin], [], [], 0.1)
        if ready:
            # Lese en linje fra stdin
            line = sys.stdin.readline().strip()
            if line.upper() == 'D':
                # b) Generer en tilfeldig bar-verdi
                bar_value = round(random.uniform(3.0, 9.0), 2)

                # c) Lagre i en CSV-lignende fil (valgfritt)
                timestamp = time.strftime("%H:%M:%S")
                with open("bar_values.csv", "a") as f:
                    f.write(f"{timestamp},{bar_value}\n")

                # d) Send bar-verdi til Qt-klienten
                msg = f"{timestamp},{bar_value}\n"
                conn.sendall(msg.encode('utf-8'))

                print(f"[SIMULERT] Sendte bar-verdi: {msg.strip()}")
        else:
            # Ingen tastetrykk -> gjør ingenting i denne loopen
            pass

    # Avslutter aldri i dette eksempelet, men du kan avbryte med Ctrl+C

if __name__ == "__main__":
    main()
