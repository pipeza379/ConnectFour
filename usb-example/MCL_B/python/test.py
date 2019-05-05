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

    # LDR sensor
    print("LDR0 : ", peri.get_light())
    # peri.send_data(peri.get_light())
    sleep(0.5)
