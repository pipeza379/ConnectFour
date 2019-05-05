from practicum import McuBoard, find_mcu_boards, PeriBoard
from time import sleep

devices = find_mcu_boards()
mcu = McuBoard(devices[0])

print("*** Practicum board found")
# print("*** Manufacturer: %s" % \
#         mcu.handle.getString(mcu.device.iManufacturer, 256))
print("*** Product: %s" % \
        mcu.handle.getString(mcu.device.iProduct, 256))


peri = PeriBoard(mcu)

a = 1


while(a != -1):
    # a=int(input("index: "))
    # b=int(input("value: "))
    # peri.set_led(a,b)

    # switch push 0-6
    print("Switch 0 : ", peri.get_switch(0)) #PB0
    print("Switch 1 : ", peri.get_switch(1)) #PB1
    print("Switch 2 : ", peri.get_switch(2)) #PB2
    # print("Switch 3 : ", peri.get_switch(3)) #PB3
    # print("Switch 4 : ", peri.get_switch(4)) #PB4
    # print("Switch 5 : ", peri.get_switch(5)) #PB5

    # print("Reset 0 : ", peri.get_reset())

    # LDR 0-5
    print("LIGHT 0: ",peri.get_light(0)) #getlight0 PC0
    print("LIGHT 1: ",peri.get_light(2)) #getlight2 PC2
    print("LIGHT 2: ",peri.get_light(4)) #getlight4 PC4
    # #LDR from board B
    # print("LIGHT 3 : ", peri.get_data(0)) #DATA0 PD0
    # print("LIGHT 4 : ", peri.get_data(1)) #DATA1 PD1
    # print("LIGHT 5 : ", peri.get_data(5)) #DATA5 PD5

    sleep(0.1)
