"""
FIX Data Generator
------------------
This script generates synthetic financial messages following the FIX 4.2 protocol.
It creates a dataset of 100,000 orders for ASML stock to be used as a 
benchmark for a high-performance C++ parser.

Output: test_data.txt
"""

import random

with open("test_data.txt", "w") as f:
    for i in range(1000000): 
        side = random.choice(['1', '2'])  
        price = round(random.uniform(850.00, 950.00), 2) 
        qty = random.randint(1, 1000)      

        #FIX message layout
        message = f"8=FIX.4.2|35=D|49=ME|56=EXCHANGE|55=ASML|54={side}|38={qty}|44={price}|"
        f.write(message + "\n")

print("File successfully created with 1,000,000 lines")