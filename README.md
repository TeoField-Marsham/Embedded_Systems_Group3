# Embedded Sensing Systems - Challenge Task: Group3
Ferhat Arslan, Teo Field-Marsham and Thiago Teixeira dos Santos

This repository implements our solution to the Challenge Task: Smart Mailbox System. We first describe our methodology, the requirements of the project and our initial ideas to meet them. Then we detail, explain, and justify our final implementation and also provide some possible further expansions of our system. Lastly, we provide instructions to help you build and run our project yourself. 

### Table of Contents

- [Initial Ideas](#initial-ideas) 
  - [The Requirements](#the-requirements)   
  - [Methodology](#methodology)  
  - [First Setup](#first-setup)  
- [Final Implementation Overview](#final-implementation-overview)  
  - [Hardware](#hardware)  
    - [Hardware Components List](#hardware-components-list)  
  - [Software](#software)  
    - [Script List](#script-list)  
- [Constraints and Further Improvements](#constraints-and-further-improvements)  
  - [Further Improvements](#further-improvements)  
- [Running the Project Yourself](#running-the-project-yourself)

## Initial ideas

### The requirements
The idea is to design a system that can detect when new parcels are deposited in the mailbox. The system should be placed within the mailbox to monitor when letters and parcels are delivered, and update via a (lightweight) wireless protocol a dashboard on a backend system. The system should also notify a user when a new parcel arrives.

![SmartMail](/images/SmartMail.png "SmartMail")

### Methodology

When we initially started this project, we did not yet know all of the hardware and software components we would have access to, to complete the task. We were provided a list of requirements, but no complete list of tools we would have at our disposal to fulfill them. These were given to us gradually as we learnt about them each week in the lectures, starting out with hardware units and then shifting towards software designs. This naturally led to us implementing a top-down design process. First we sketched out how our entire system was going to be structured and then we slowly defined what exactly each hardware component would be and which software we would use for the interactions between them. 

### First setup

Our first step was coming up with the basic framework of our project. This involved simply translating the requirements into a real-world map which we could then fill out in detail as the lectures went on. 

For our first solution we decided to only work with the upper mailbox which is the intake for letters only. This would require two hardware modules: one in the mailbox and one in the house. The module in the mailbox, is responsible for sensing incoming mail and transmitting this information to the module in the house. The module in the house receives information from the mailbox and then notifies the resident, via email, that they have received physical mail. It also maintains a log of all mail ever received and makes this information accessible via a dashboard. The reason for the dual module scheme is because from the beginning we knew that the home wifi network, required for sending an email, would not be able to reach the mailbox.

- In the mailbox: 
    - **Hardware:** battery/power source, micro-controller, radio/transmission sender, sensors
    - **Software:** motion sensor register script, sender
- In the house: 
    - **Hardware:** radio/transmission receiver, micro-controller, battery/power source (a laptop or even wall outlet)
    - **Software:** receiver, backend functions script (contains the email forwarding system, logs and any other helper tools) 

## Final implementation overview

Our final implementation ended up closely following most of our initial ideas. We took our first schemes and concepts and concretized them by clearly defining the exact hardware components and writing our code. Considering that we approached this project from a top-down perspective, our initial setup was almost perfect and encompassed all hardware and software components we needed aside from one feature: the reset button and it's corresponding software functionality. 

The requirements state the need for a dashboard to log ever piece of mail that enters the mailbox, however this running total does not need to be sent to the user every time a letter enters the mailbox. The reset button feature is our solution to separating the dashboard counter from the email notification system's counter and will be explained in more detail in the [software section](#software). 

In the following sections the exact specifications, functionalities, and rationale for each individual hardware and software component will be discussed.

### Hardware

#### Hardware components list:

###### Mailbox Module
| Quantity | Component                                                     |
|----------|---------------------------------------------------------------|
| 1×       | Solderless Breadboard, MB-102 (830 tie-point)                 |
| 1×       | PIR Motion Sensor Module, HC-SR501                            |
| 1×       | LilyGo TTGO Module: LoRa32 868/915 MHz with antenna           |
| 1×       | Pin Header                                                    |
| 1×       | Set of F–M and M–M DuPont Jumper Wires                        |
| 4×       | Resistors                                                     |
| 1×       | Powerbank (MicroUSB)                                          |
| 1×       | Button Switch                                                 |

###### House Module

| Quantity | Component                                                     |
|----------|---------------------------------------------------------------|
| 1×       | LilyGo TTGO Module: LoRa32 868/915 MHz with antenna           |
| 1×       | MicroUSB cable and charger                                    |

#### Wiring and Assembly

We start with an easy-to-rewire, flexible solderless breadboard (MB-102). To detect mail or letters, we use a PIR motion sensor module, which measures the infrared light emitted by objects within its field of vision. An alternative would be to use an ultrasonic sensor, which may be suitable for boxes. However, as the letters are quite flat, they could be difficult to recognise. Also, the tolerance is about 3 mm, meaning that some letters may not be recognised. For this reason, we opted for the PIR motion sensor, which consumes little power.

Next, we used the LilyGo, which uses the LoRa32 module and has a long range, making it ideal for a letterbox outside the house or on the street. Alternatives would include using BLE, Wi-Fi or ZigBee. However, due to their shorter range, LoRa was the best option for us, as it has a range of up to 5 km in a dense environment. In addition, the module enters deep sleep mode between transmissions, consuming less than 10 µA of quiescent current and ensuring that the overall energy consumption remains extremely low.

For wiring and assembly, you can take the components used in the hardware components and assemble them as follows:

![Circuit](/images/circuit.jpeg "Circuit")

First wire the hardware components for the mailbox module using the supplied circuit diagramm. Use Wire Jumpers to connect the PIR Motion Sensor to connect to the LilYGo with the antenna and then connect the LilyGo with the breadboard also with wire jumpers. On the breadboard you add a button, make sure to use resistors. Then for the power supply of the LilyGo you can use a power bank and connect it via MicroUSB. One disadvantage of using a power bank is that the capacity of the Li-ion battery decreases in winter when the temperature is below 0°C, and the battery ages faster at temperatures above 40°C. Overall, using a power bank for power supply is easy, cheap and safe. As the power consumption was quite low, the power bank kept switching off. To prevent this, we used four additional resistors to increase the power consumption above the minimum required.

![Circuit](/images/wiring.png "Circuit")

The next step is to insert the mailbox module into the mailbox. Make sure that the motion sensor is optimally positioned and that the button is at the front for easier access.

 ![Mailbox](/images/mailbox.jpeg  "Mailbox")

The mailbox module must now be connected to another LilyGo module, known as the house module, which can be placed inside your house. This enables the mailbox module to send packets to the house module via LoRa. The LilyGo can then be connected to your home's WiFi and send the received information by email or display it on the web server. The house module can be powered by simply connecting it to a charger using a Micro USB cable.

 ![house](/images/house.jpeg  "house")


### Software

The software components of our system are critical as they are what allows our hardware to communicate with each other and fulfill their functions so that they are interpretable by human users. They also serve as the overarching framework of the system, integrating its various components and offering a comprehensive perspective on its overall architecture and functionality.

We tried to keep our software as simple as possible. Since the two hardware modules are ESP32 based, the code is written in C++. Each ESP32 can only run one file at a time, so there are only two files in our system. The mailbox module's ESP32 uses the `lora_projectsender.ino` and the house module's ESP32 uses the `lora_projectreciever.ino`. Together they establish a unidirectional, event-driven pipeline: sensor → LoRa → Wi-Fi → user.

**Mailbox module software (event sender)**

The mailbox ESP32’s only job is to notice physical events and translate them into the smallest possible LoRa messages. Two GPIO interrupts handle those events:

- **PIR motion sensor:** triggers when the mail slot flap moves, telling the user “something entered the box”.

- **Reset button:** pressed by the resident when the mailbox is emptied.

Using interrupts instead of polling allows the microcontroller to stay in a low-power idle state most of the time and the current spikes only when real work is required. A lightweight software debounce (3 seconds for the reset button, 5 seconds for the motion sensor) filters non-intended activations and prevents the two events from colliding with one another when the box is opened and immediately emptied.

When an interrupt is accepted the LoRa antenna transmits one of two plain-text tokens:

| Token | Meaning | When sent |
|-|-|-|
| `newmail` | A letter was detected | Motion interrupt fires and no reset follows in the next 5 s |
| `reset` | Mail has been collected | Reset button interrupt fires |

Using single-word packets keeps transmission time below a millisecond at 915 MHz, maximizing battery life and giving LoRa’s forward-error-correction an easier job. 

**House module software (event receiver, notifier, and dashboard)**

The indoor ESP32 has three roles:

- **LoRa receiver:** Waits for packets, parses the payload, and time-stamps the event as soon as one arrives.

- **Notifier:** For each distinct event it assembles an email via ESP Mail Client and relays it through our own Gmail service's SMTP gateway.

- **Dashboard:** At the same time the event is appended to an in-memory eventLog string. A HTTP server can display that log to any browser on the local network. The page polls the root URL every five seconds, so the table of deliveries updates in real time without the need for websockets or a heavy framework.

A single global counter `mailCount` keeps track of how many uncollected items are in the mailbox. Receiving `newmail` increments the counter and triggers a “You received mail!” email. If the user presses the reset button after collecting any mail from the mailbox, a `reset` sets it back to zero and triggers a “Mail collected!” email. This reset only effects the counter for the email service and not the dashboard's log which never gets reset. This allows a complete history of all events to be stored in the log, whereas emails only ever notify the user of exactly how many letter are in the mailbox at that time. Additionally, this counter is maintained only in RAM so it resets automatically on power-cycle, as persisting it to flash would wear the cells and is unnecessary for day-to-day use.

Some key concluding points:

- Whilst LoRa's range is measured in kilometers, realistically, more than 50 meters between your house and mailbox is unlikely, so this is definitely more than enough. LoRa's bit rate may be lower than for example MQTT, however given how small and infrequent our messages are, this is not a problem. Additionally, LoRa uses very little power helping to increase the time spans between needing to charge the power bank (for the mailbox module). 

- We rely only on two non-standard libraries [LoRa by sandeepmistry](https://github.com/sandeepmistry/arduino-LoRa) and [ESP Mail Client by mobizt](https://github.com/mobizt/ESP-Mail-Client). Both are actively maintained and small enough to fit alongside the HTTPS server in flash.

- Keeping Wi-Fi and any credentials out of the mailbox module's software means a malicious person cannot gain access to the home network by breaking into the mailbox. 

- The house unit doesn't need sleep logic as it is plugged into a constant source of power (laptop or wall outlet), simplifying the software regarding TLS sockets and HTTP requests.

- If the sender battery dies, no packets are received and the log's list will remain unchanged. The user will also notice that they don't receive a “Mail collected!” email. Both of these indirectly alert the user to the fact that they need to recharge the power bank. 

- If the Wi-Fi network does down, whilst email notifications will not work, the log will still be updated and allow the user to check if they received any letters during this period.

#### Script list:
    - Mailbox module
        lora_projectsender.ino
    - House module
        lora_projectreciever.ino


### Constraints and further improvements

Given that this was a university level project, we had some constraints when it came to our actual implementation. These were primarily hardware based, because we were provided with the hardware to beat the challenge and only had limited ability to choose our own. Software is more flexible since we were allowed to use whichever libraries we wanted and there were not limits or rules imposed upon us. 

What this lead to was primarily a lack of aesthetics and reproducibility. Our modules were exposed motherboards, wires, breadboards and sensors and not fully enclosed modules. This was not a big concern for the house module as there is more space, however the mailbox module does take up some space within the mailbox and could also be knocked around if large letters are pushed into the mailbox. With the right equipment, the work around we theorized for this issue, would be containing the whole module within a long, thin rectangular box, with a window for the motion sensor to look out and a port to charge the battery from. This would allow the entire module to be slid into the very back of the mailbox taking up as little space as possible with no wires to catch on anything. If the mailbox is metal, one could even use magnets to ensure that it cannot be knocked around. 

Additionally, our mailbox module had such a low power draw, that the provided power bank would automatically turn off completely after a few seconds of being switched on. In order to work around this, we first increased the power draw by simply adding lots of LED lights to the breadboard, but then we refined the system by replacing all the LEDs with resistors. 


#### Further improvements

- **Symmetric key of encryption:**
    In our current implementation, the module in the letter box is locked, cannot be accessed by unauthorized persons, and also contains not valuable information. The module in the house is also locked or at least cannot be accessed by unauthorized persons without the user/owner knowing. Additionally, the owner/users email mailbox can be assumed to be secure and therefore the only security weakness in our network is the LoRa transmission between the mailbox module and the house module. 
    
    Our solution to this would be implementing a symmetric key encryption scheme for all communication between the mailbox module and the house module. This would ensure that even if an attacker were to intercept the LoRa messages, they would not be able to decipher the contents without access to the shared secret key. The key can be pre-shared securely during installation, as both modules are physically secured and typically accessed only by authorized personnel. This method is computationally efficient, making it suitable for our low-power embedded system. In addition, periodic key rotation or using a simple key exchange protocol at regular pre-determined intervals could further enhance the security without introducing significant overhead.

    However, the need of such encryption is debatable, because the only data being transmitted are two messages "the sensor was triggered" and "the reset button was pressed". So even if an attacker intercepts a perfectly encrypted message, they know the time when they intercepted the message and they know there are only two possible things it could say. For true message-level confidentiality, the system would need to regularly transmit encrypted "cover messages" at random intervals, making it impossible for an attacker to distinguish between real and decoy messages without breaking the encryption.

- **Additional sensors:**
    Currently we have a single sensor in the mailbox to detect incoming mail. This sensor registers when the mail slot opens or when a letter enters the mailbox, but it doesn't indicate how much mail was delivered. An additional weight sensor or ultrasonic depth sensor would provide more detailed information such as the quantity or volume of mail inside, instead of simply detecting whether mail was received. 

    The standard Swiss mailbox has two compartments, an upper one for letters and a lower one with a larger opening for packages. Currently, we can only detect whether anything is entered into the upper compartment, however if we were allowed to customize the actual mailbox by drilling holes into it, then we could add sensors into the lower compartment as well. Theoretically we could take our current mailbox module and duplicate it and put it in the lower mailbox, however this solution would be clunky and also take up a lot of space that would limit the size of packages that can go into the mailbox. Drilling a small hole between the two compartments at the very back of the mailbox would allow running wires from the upper box to the lower one and connecting them to a single motion sensor. Whilst this would technically increase power draw from the single battery, there would only be a single motion sensor in the lower compartment and not 

- **Power options:**
    Currently our mailbox module receives power from a power bank and our house module receives power from a wall outlet. The mailbox module could be improved only by changing the exact power bank or battery, considering that the typical Swiss mailboxes has no access to any kind of power grid. Depending on the exact specifications of the mailbox module (the amount of sensors, whether the mailbox is indoors or outdoors, etc.) a different battery could improve performance and increase time between charging. If one wanted a fixed power supply, one could technically run a long cable to the mailbox from the nearest outlet (and drill a hole to get the power cable into the mail compartment), however this would be clunky, ugly and simply unnecessary. 

## Running the project yourself

To run our project yourself, you simply need the code contained in this Github repository and the hardware components specified in our [hardware section](#hardware).

Before starting make sure you have Arduino IDE installed on a computer and that the `Tools/Board:` is set to "ESP32 Dev Module". Additionally when plugging in boards, make sure to select the right COM port under `Tools/Port:`.

**Step 1:** 
- Replicate the mailbox module and house module. The complete list of hardware components for each module can be found in the [hardware section](#hardware). Once you have all of the components, use the circuit diagrams and images to assemble each module.
- Make sure the power bank is fully charged.
- If you notice your powerbank turns off after a few seconds, it may be due to it detecting too low current usage. In order to increase current usage, you can add resistors connected to the Vcc and GND on the protoboard.

**Step 2:** 
- Clone this repository.

**Step 3:** 
- Plug the mailbox module's ESP32 into your laptop (detach the power bank for this). 
- Copy the lora_projectsender into Arduino IDE (no edits to code required).
- Compile and upload the code onto the ESP32, detach the ESP32 from the laptop and reattach the power bank.

**Step 4:** 
- Plug the house module's ESP32 into your laptop. 
- Copy the lora_projectreciever into Arduino IDE. Read the code: download any libraries specified in the comments and fill out all fields the comments specify you to fill out. 
- Compile and upload the code onto the ESP32.
- Take note of the IP address shown in the terminal.

**Step 5:** 
- Place the mailbox module in your mailbox as far back as you can making sure the sensor is pointing towards the slot.

**Step 6:**
- Connect the receiver to an electrical plug (or the computer)
- On a device under the same wi-fi as the receiver, you can insert the IP address noted in Step 4 into a browser to access the webserver. 

**Step 7: Receiving Mail**
- Receiving a letter should send an email to the registered email address at the receiver and also increase the counter at the webserver.

**Step 8: Retrieving Mail**
- To retrieve your mail, simply open the compartment and press the button within 10 seconds. This should send an email to the registered email address at the receiver notifying about the mail retrieval and also set the counter at the webserver back to zero.
