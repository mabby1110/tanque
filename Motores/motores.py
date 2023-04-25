# librerias
import RPi.GPIO as GPIO
import os
import getch
import time

# hyperparametros
MODO = True             # True: RC   False: Sigue lineas

# GPIO Motores
ena = 5                 # Motores
enb = 6                 # Motores
inx = [21, 20, 16, 12]  # Modulo motores (controlador)

# GPIO sensores
sensores = [19, 13]

# Configuracion GPIO sensores
GPIO.setmode(GPIO.BCM)

GPIO.setup(sensores[0], GPIO.IN)
GPIO.setup(sensores[1], GPIO.IN)

# Configuracion GPIO motores
GPIO.setmode(GPIO.BCM)

GPIO.setup(ena, GPIO.OUT)
GPIO.setup(enb, GPIO.OUT)

GPIO.setup(inx[0], GPIO.OUT)
GPIO.setup(inx[1], GPIO.OUT)
GPIO.setup(inx[2], GPIO.OUT)
GPIO.setup(inx[3], GPIO.OUT)

pwm_a = GPIO.PWM(ena, 100)
pwm_b = GPIO.PWM(enb, 100)

# Control Velocidad
def Control_vel(a, b):
    pwm_a.start(a)
    pwm_b.start(b)

# Funciones que controlan el movimiento de los motores
def Avanzar():
    print('Avanzando ...')
    Control_vel(50, 50)
    # motor 1
    GPIO.output(inx[0], False)
    GPIO.output(inx[1], True)

    # motor 2
    GPIO.output(inx[2], True)
    GPIO.output(inx[3], False)

def Girar_der():
    print('girando der ...')
    Control_vel(10, 30)
    # motor 1
    GPIO.output(inx[0], True)
    GPIO.output(inx[1], False)

    # motor 2
    GPIO.output(inx[2], True)
    GPIO.output(inx[3], False)

def Girar_izq():
    print('girando izq ...')
    Control_vel(30, 10)
    # motor 1
    GPIO.output(inx[0], False)
    GPIO.output(inx[1], True)

    # motor 2
    GPIO.output(inx[2], False)
    GPIO.output(inx[3], True)

def Frenar():
    print('Frenando ...')
    GPIO.output(inx[0], False)
    GPIO.output(inx[1], False)
    GPIO.output(inx[2], False)
    GPIO.output(inx[3], False)

# Estetica
os.system('clear')

print('Controles\n')
print('avanzar\tW\tFrenar\tS\n')
print('izq\tA\tder\tD\n')
print('--------------------')

# Main
try:
    while 1:
            
        if MODO:
            print('MODO: motores')
            cmd = getch.getch()
            if cmd == 'm':
                MODO = not MODO
                os.system('clear')
            # Getch lee el teclado
            if cmd == 'a':
                Girar_izq()
            elif cmd == 'd':
                Girar_der()
            elif cmd == 'w':
                Avanzar()
            elif cmd == 's':
                Frenar()

        if not MODO:
            if GPIO.input(13) and not GPIO.input(19):          # Sensor der
                Girar_izq()
            elif not GPIO.input(13) and GPIO.input(19):        # Sensor izq
                Girar_der()
            elif GPIO.input(13) and GPIO.input(19):
                Avanzar()
            os.system('clear')
except:
    pwm_a.stop()
    pwm_b.stop()
    GPIO.cleanup()
    print('\n\nbye\n')