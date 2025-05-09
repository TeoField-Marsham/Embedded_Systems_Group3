# Embedded Sensing Systems - Challenge Task: Group3
Ferhat Arslan, Teo Field-Marsham and Thiago Teixeira dos Santos

This repository implements our solution to the Challenge Task: Smart Mailbox System. 

## Intital ideas:

### Methodology: 

When we initally started this project, we did not yet know all of the hardware and software components we would have access to, to complete the task. We were provided a list of requirements, but no complete list of tools we would have at our disposal to fulfill them. These we were given gradually as we learnt about them each week in the lectures, starting out with hardware units and then shifting towards software designs. This naturally led to us implementing a top-down design process. First we sketched out how our entire system was going to be structured and then we slowly defined what exactly each hardware component would be and which software we would use for the interactions between them. 

#### The requirements: 
The idea is to design a system that can detect when new parcels are deposited in the mailbox. The system should be placed within the mailbox to monitor when letters and parcels are delivered, and update via a (lightweight) wireless protocol a dashboard on a backend system. The system should also notify a user when a new parcel arrives.

### First setup:

Our first step was coming up with the basic framework of our project. This involved simply translating the requirements into a real-world map which we could then fill out in detail as the lectures went on. 

- In the mailbox: 
    - Hardware: battery (power bank), esp32, lora antenna, motion sensor
    - Software: motion sensor reagister scrip, lora sender
- In the house: 
    - Hardware: lora antenna and esp32 (plugged into a laptop for power)
    - Software: lora receiver, email forwarding system

## Final implementation overview

...

## Hardware

..

## Software

...

## Conclusion

...

### Further improvments

- Some form symetteric key of encryption:
