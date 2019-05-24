01204223 Practicum for Computer Engineering
Department of Computer Engineering, Faculty of Engineering, Kasetsart University.

###ConnectFour###

code with Arduino

Equipment
	
	>> TCRT5000L x 6
	>> WS2812B 30LEDS x6
	>> switch x 7
	>> switch two state x 1 
	>> Board Practicum

Function & Rule
	
	1.กด BTN0 , BTN1 ในการควบคุมซ้ายขวาเพื่อเลือกสีที่จะใช้ในการเล่น และกดปุ่ม BTN2 เพื่อยืนยัน (หากไม่ต้องการเลือกสามารถกดปุ่ม BTN2 จะถือเป็นการ  set default และสามารถกดค้างเพื่อเลื่อนสีอย่างรวดเร็วได้)
	2.เมื่อเลื่อนนิ้วเข้าใกล้ปุ่ม จะทำการ highlight แถวที่จะลงเหรียญ ทั้งนี้จะต้องกดและปล่อยปุ่มเพื่อลงเหรียญ
	3.เมื่อมีผู้ชนะ จะแสดงสีของผู้ชนะ จากนั้นจะเคลียร์กระดาน ผู้แพ้จะเป็นคนเริ่มก่อน หากเสมอจะแสดงสีของสองฝ่ายอย่างละครึ่ง
	4.กด Reset เพื่อตั้งค่าและเริ่มต้นใหม่
	
Source Code
	led/led.ino

Board Schemetic
	schematic.png

Developer
	Chisatnupong
	Kittapon Junsupakul 6010504651