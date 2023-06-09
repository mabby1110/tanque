# librerias
import RPi.GPIO as GPIO
import os


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
    Control_vel(20, 20)
    # motor 1
    GPIO.output(inx[0], False)
    GPIO.output(inx[1], True)

    # motor 2
    GPIO.output(inx[2], True)
    GPIO.output(inx[3], False)

def Girar_der():
    print('girando der ...')
    Control_vel(20, 20)
    # motor 1
    GPIO.output(inx[0], False)
    GPIO.output(inx[1], True)

    # motor 2
    GPIO.output(inx[2], False)
    GPIO.output(inx[3], True)

def Girar_izq():
    print('girando izq ...')
    Control_vel(20, 20)
    # motor 1
    GPIO.output(inx[0], True)
    GPIO.output(inx[1], False)

    # motor 2
    GPIO.output(inx[2], True)
    GPIO.output(inx[3], False)

# borra la terminal
os.system('clear')

# Main
try:
    while 1:
        if GPIO.input(13) and not GPIO.input(19):          # Sensor izq
            Girar_izq()
        elif not GPIO.input(13) and GPIO.input(19):        # Sensor der
            Girar_der()
        elif GPIO.input(13) and GPIO.input(19):
            Avanzar()
        os.system('clear')
except:
    pwm_a.stop()
    pwm_b.stop()
    GPIO.cleanup()
    print('\n\nAdiós\n')