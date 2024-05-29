# Steganography Program

## Description

This project 

The `main.py` program uses the pygame library to implement the driving game, complete with drifting mechanics. The `agent.py` program uses Deep Q Learning to iteratively improve a Neural Network model, making the AI better at the driving game. The logic of the Neural Network is found in `model.py`, and the models are stored in the `/model` folder.

## Installation

1. Clone the repository
2. [Install a C compiler]
3. Navigate to the repository
```bash
cd Steganography
```
4. Compile the code (e.g. gcc steg.c)

## Running the project

- Navigate to the root of the project
- To hide a message in an image, run:
```bash
./<executable-name> -e -f <image_path> [-k <key>] [-p <password>] message
```
- To decrypt an image, run:
```bash
./<executable-name> -d -f <image_path> [-k <key>] [-p <password>]
```

## Options:
-e                         hide the provided message into the provided image
-d                         decrypt the message from the provided image
-f [file]                  use this file to hide/decrypt 
-p [password]              use this password to hide/decrypt 
-k [key]                   use this key to hide/decrypt - key is 256 bit written as HEX


## Questions or Feedback?

If you have any questions, encounter any issues, or have feedback to share, please don't hesitate to [open an issue](https://github.com/ShayanHaghighi/Steganography/issues) <!--or [reach out to me](link_to_contact_information)-->. Your input is invaluable and helps improve this project!

## Thank You

Thank you for visiting this project and for your interest in its development. Your support means a lot! If you find this project useful, consider giving it a ⭐️ or sharing it with others who might enjoy from it.

Happy coding