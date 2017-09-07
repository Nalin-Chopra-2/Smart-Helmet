# Smart-Helmet
Arduino Smart Helmet with Accelerometer

This was an engineering design mentored project that I worked on during the summer of 2017 at UC Berkeley. I presented this prototype alongside teammates in a design showcase, where it was awarded runner-up honors for the Judge's choice award with a judging panel composed of representatives from Chevron, PG&E, AutoDesk, and UC Berkeley Faculty.

One of the biggest issues in football and in high-contact sports today is that concussions and other head injuries are qualitatively diagnosed by humans. Players often suffer multiple concussions or are misdiagnosed, causing them to suffer later on or to miss extended amounts of playing time due to injury.

This code used in a "Smart" Helmet capable of detecting major changes in acceleration and resultingly whether or not a player is concussed, without resulting on sometimes biased and inconsistent diagnoses from human training staff.


This code was uploaded on a mini Arduino and was connected with an accelerometer and an XBee wireless transmitter. I worked most specifically on getting the the Arduino to read and record the accelerometers data, and based on whether or not the acceleration in a given direction (x, y, or z) exceeded a certain threshold, the serial monitor output would print "Concussed in *insert axis*". Rotational acceleration was also recorded to with a similar threshold concept to detect if the "twisting" motion in a certain direction could potentially be harmful to a player. A hit counter was also implemented to see how many hits a player has sustained over a given interval.

The biggest challenge with this project was implementing the wireless transmitter, which still has a few bugs. 

Alongside the mini Arduino, accelerometer and Wireless transmitter, I 3-D printed the cross section of a football helmet and embedded the hardware module with this components. Overall the project was extremely fun to work on and I love that I was able to combine my passion for sports and specifically football with engineering.

