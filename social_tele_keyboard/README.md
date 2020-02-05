# Instruction manual

## __Key event__

* ### Joint 
    * __"d"__ : is home key. (pan--->0, tilt--->0, arm--->0)
    * Pan   
        * __"f"__ : right    
        * __"s"__ : left

    * Tilt  
        * __"c"__ : down
        * __"e"__ : up

    * Arm   
        * __"q"__ : move
        * __"w"__ : stop

* ### Led   
    * __"r"__ : red (on/off)    
    * __"g"__ : green (on/off)
    * __"b"__ : blue (on/off)
    * __"o"__ : off

* ### Test   
    * __"t"__ : test mode (on/off)    

## __Publish__
* url: /TinyCore/Tele/Joint/commands   
msg type: std_msgs/Int32MultiArray  
desc: [0] pan_angle, [1] tilt_angle, [2] arm_speed

* url: /TinyCore/Tele/LED/commands  
msg type: std_msgs/String  
desc: "red", "green", "blue", "off"