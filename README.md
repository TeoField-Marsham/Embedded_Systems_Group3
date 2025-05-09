# Embedded Sensing Systems - Challenge Task: Group3
Ferhat Arslan, Teo Field-Marsham and Thiago Teixeira dos Santos

This repository implements our solution to the Challenge Task: Smart Mailbox System. 

## Initial ideas

### Methodology

When we initially started this project, we did not yet know all of the hardware and software components we would have access to, to complete the task. We were provided a list of requirements, but no complete list of tools we would have at our disposal to fulfill them. These we were given gradually as we learnt about them each week in the lectures, starting out with hardware units and then shifting towards software designs. This naturally led to us implementing a top-down design process. First we sketched out how our entire system was going to be structured and then we slowly defined what exactly each hardware component would be and which software we would use for the interactions between them. 

#### The requirements
The idea is to design a system that can detect when new parcels are deposited in the mailbox. The system should be placed within the mailbox to monitor when letters and parcels are delivered, and update via a (lightweight) wireless protocol a dashboard on a backend system. The system should also notify a user when a new parcel arrives.

### First setup

Our first step was coming up with the basic framework of our project. This involved simply translating the requirements into a real-world map which we could then fill out in detail as the lectures went on. 

For our first solution we decided to only work with the upper mailbox which is the intake for letters only. This would require two hardware modules: one in the mailbox and one in the house. The module in the mailbox, is responsible for sensing incoming mail and transmitting this information to the module in the house. The module in the house receives information from the mailbox and then notifies the resident, via email, that they have received physical mail. The reason for the dual module setup, is because the home wifi network, required for sending an email, does not reach the mailbox. 

- In the mailbox: 
    - Hardware: battery (power bank), esp32, LoRa antenna, motion sensor, reset button
    - Software: motion sensor register scrip, LoRa sender
- In the house: 
    - Hardware: LoRa antenna, esp32, power cable connected to a laptop for power
    - Software: LoRa receiver, User Interface (contains the email forwarding system, logs and our helper tools) 

## Final implementation overview

...

## Hardware

...

WHEN DISCUSSING POWER MENTION THAT SOME SWISS MAILBOXES ARE OUTDOORS AND IN THE WINTER IF IT SNOWS OR IN THE SUMMER WHEN ITS VERY HOT, THIS COULD EFFECT THE BATTERY

## Software

...

## Conclusion

...

### Constraints

Given that this was a university level project, we had some constraints when it came to our actual implementation. These were primarily hardware based, because we were provided with the hardware to beat the challenge and not allowed to choose our own. Software is more flexible since we were allowed to use whichever libraries we wanted and there were not limits or rules imposed upon us. 

We were working with a breadboard and wires so our modules were not aesthetically pleasing. 

...

### Further improvements

- **Symmetric key of encryption:**
    In our current implementation, the module in the letter box is locked and cannot be accessed by unauthorized persons. The module in the house is also locked or at least cannot be accessed by unauthorized persons without the user/owner knowing. Additionally, the owner/users email mailbox can be assumed to be secure and therefore the only security weakness in our network is the LoRa transmission between the mailbox module and the house module. 
    
    Our solution to this would be implementing a symmetric key encryption scheme for all communication between the mailbox module and the house module. This would ensure that even if an attacker were to intercept the LoRa messages, they would not be able to decipher the contents without access to the shared secret key. The key can be pre-shared securely during installation, as both modules are physically secured and typically accessed only by authorized personnel. This method is computationally efficient, making it suitable for our low-power embedded system. In addition, periodic key rotation or using a simple key exchange protocol at regular pre-determined intervals could further enhance the security without introducing significant overhead.

    However, the need of such encryption is debatable, because the only data being transmitted is a simple message "the sensor was triggered." So even if an attacker intercepts a perfectly encrypted message, they know the time when they intercepted the message and they know the only thing it could possibly say is that the sensor was just triggered adn therefor they can know they exact same thing that the real user knows. For true message-level confidentiality, the system would need to regularly transmit encrypted "cover messages" at random intervals, making it impossible for an attacker to distinguish between real and decoy messages without breaking the encryption.

- **Additional sensors:**
    Currently we have a single sensor in the mailbox to detect incoming mail. This sensor registers when the mail slot opens or when a letter enters the mailbox, but it doesn't indicate how much mail was delivered. An additional weight sensor or ultrasonic depth sensor would provide more detailed information such as the quantity or volume of mail inside, instead of simply detecting whether mail was received. 

    The standard Swiss mailbox has two compartments, an upper one for letters and a lower one with a larger opening for packages. Currently, we can only detect whether anything is entered into the upper compartment, however if we were allowed to customize the actual mailbox by drilling holes into it, then we could add sensors into the lower compartment as well. Theoretically we could take our current mailbox module and duplicate it and put it in the lower mailbox, however this solution would be clunky and also take up a lot of space that would limit the size of packages that can go into the mailbox. Drilling a small hole between the two compartments at the very back of the mailbox would allow running wires from the upper box to the lower one and connecting them to a single motion sensor. Whilst this would technically increase power draw from the single battery, there would only be a single motion sensor in the lower compartment and not 

- **Power options:**
    Currently our mailbox module receives power from a power bank and our house module receives power from a laptop. The mailbox module could be improved only by changing the exact power bank or battery, considering that the typical Swiss mailboxes has no access to any kind of power grid. Depending on the exact specifications of the mailbox module (the amount of sensors, whether the mailbox is indoors or outdoors, etc.) a different battery could improve performance and increase time between charging. If one wanted a fixed power supply, one could technically run a long cable to the mailbox from the nearest outlet (and drill a hole to get the power cable into the mail compartment), however this would be clunky, ugly and simply unnecessary. 

    The house module could however be integrated in a more efficient way. It is much smaller, composed of only an esp32 with an antenna attached to it and a power cable and so it could easily be plugged into any wall outlet. As long as it is still in range of the mailbox, it doesn't matter where exactly it is located in the house. The only concern would be if a user wanted to access the logs, however in a finished product, there wouldn't be any reason for this. 
