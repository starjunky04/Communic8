# Communic8

## Description

My project, ‘Communic8’, is an interactive robotic octopus. It uses brain waves gathered from a headset the user wears to respond to their neurological state, whether by changing its colour or texture or by moving its tentacles. The robot aims to mimic the behaviours of a real octopus.

I am autistic and dyslexic. I wasn’t diagnosed until later in life, but I have always had trouble communicating and expressing myself through traditional English. I could never find the language to express or communicate my thoughts, ideas and facts, and I honestly find it easier to communicate what I mean through sound effects. My own personal form of communication tends to make people percieve me as less intelligent even though this is not the case. Even after my diagnosis, I felt insecure and through research discovered the stigma surrounding neurodivergent individuals in particular when they commmunicate through unconventional methods.

This project was inspired by my own challenges and aims to encourage the viewer to embrace the ways they find intuitive to express themselves, to explore unconventional forms of expression by challenging pre-existing methods, and to discover a unique method of communication that is most intuitive to them.

## Features

This project is a way of visualising a users neurological reactions through a robotic octopus, mimicking the behaviour of the real creature. It can:

 * Change its colour
    * The speed at which the colours move change based on the data
    * The directional path of the colours also may change
 * Move it's tentacles/arms
    * The tentacles will curl in accordance with the data
  
## Getting Started

### Prerequisites
You will need an Arduino, Muse headband, External 5V power supply, NeoPixels, Motor driver, and 12 motors.
Arduino IDE to upload the arduino code.

### Installation

1. **Clone the repo**

   ```bash
     git clone https://git.arts.ac.uk/23027141/Communic8.git
     cd Communic8
   ```

3. **Upload the code to the Arduino**
   
   Found in: [*code/arduino-code/*](https://git.arts.ac.uk/23027141/Communic8/blob/main/code/arduino-code)
   * The arduino code **MUST** be uploaded **BEFORE** the Jupyter notebook is run.
   * Also ensure that Serial Monitor tab in Ardunio is **CLOSED**

3. **Build the circuit**
   
  ![image](https://files.catbox.moe/ymqjx1.png)

5. **Set up Muse**
   * Connect the headband to your [muse app](https://choosemuse.com/pages/app) via Bluetooth
   * In the App go to *Settings -> Developer Tools -> OSC Output*
     * Enable streaming
     * There you will find the IP Address and Port. If these are different from what is found in [*code/python-code.ipynb*](https://git.arts.ac.uk/23027141/Communic8/blob/main/code/python-code.ipynb), then edit the code to match
   * In Muse, run *Live Brainwave Powerbands*
   * Once the device is calibrated and the waves are being shown, progress to the next step

6. **Download requirements.txt**
 
   ```bash
     pip install -r requirements.txt
   ```
   
8. **Run the Jupyter Notebook**
   * Found in: [*code/python-code.ipynb*](https://git.arts.ac.uk/23027141/Communic8/blob/main/code/python-code.ipynb)
   * You should be able to see the data printing in the terminal to know if it's being streamed.

9. **Interrupt the code**
   * When finished, in Python/Jupyter Notebook, stop the code.
   * Then run the final block

  
   ```
     ser.close()
   ```
   
     To close the ports.
