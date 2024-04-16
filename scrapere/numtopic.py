import requests
from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options
import time
import pygame
import io
import os

# Function to extract name and address based on input
def get_name_and_address(data_input):
    url = "https://118.dk/search/go"
    querystring = {
        "pageSize": "25",
        "whatName": "",
        "whatCategory": "",
        "whatPhone": "",
        "whatOther": "",
        "whereStreet": "",
        "whereHouseNumber": "",
        "whereZip": "",
        "whereCity": "",
        "category": "",
        "keyword": "",
        "what": data_input,
        "where": ""
    }
    headers = {
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
        "Accept-Language": "da,en-US;q=0.9,en;q=0.8,da-DK;q=0.7",
        "Connection": "keep-alive",
        "DNT": "1",
        "Sec-Fetch-Dest": "document",
        "Sec-Fetch-Mode": "navigate",
        "Sec-Fetch-Site": "same-origin",
        "Sec-Fetch-User": "?1",
        "Upgrade-Insecure-Requests": "1",
        "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36"
    }

    response = requests.get(url, headers=headers, params=querystring)
    soup = BeautifulSoup(response.text, 'html.parser')
    
    result_block = soup.find('div', class_='result-block')
    name = result_block.find('h3').text.strip() if result_block.find('h3') else "Name not found"
    address = result_block.find('address').text.strip() if result_block.find('address') else "Address not found"
    
    return name, address

# Function to fetch and display profile pictures using pygame in a 4x5 grid
def display_profile_pictures(name):
    # Setup Chrome options
    options = Options()
    options.headless = True  # Use headless mode to avoid opening a browser window

    # Setup Service with ChromeDriverManager
    service = Service(ChromeDriverManager().install())

    # Initialize the Chrome driver
    driver = webdriver.Chrome(service=service, options=options)

    # Modify the URL to search for the specific name
    url = f'https://www.facebook.com/public/{name.replace(" ", ".")}'

    # Open the URL
    driver.get(url)
    
    # Pause to allow the page to load completely
    time.sleep(10)

    # Find all images that are likely to be profile pictures
    images = driver.find_elements(By.CSS_SELECTOR, "img._1glk._6phc.img")

    # Collect the src of the first 20 profile images
    profile_pics = [img.get_attribute('src') for img in images[:20]]

    # Close the driver
    driver.quit()

    # Start pygame
    pygame.init()
    screen_width = 1024
    screen_height = 768
    screen = pygame.display.set_mode((screen_width, screen_height))
    pygame.display.set_caption('Profile Pictures')

    # Load images
    loaded_images = []
    for src in profile_pics:
        image_response = requests.get(src)
        image_file = io.BytesIO(image_response.content)
        image = pygame.image.load(image_file)
        image = pygame.transform.scale(image, (screen_width // 4, screen_height // 5))
        loaded_images.append(image)

    # Image Rectangles for click detection
    image_rects = [
        pygame.Rect((i % 4) * (screen_width // 4), (i // 4) * (screen_height // 5), screen_width // 4, screen_height // 5)
        for i in range(len(loaded_images))
    ]

    # Display images in a 4x5 grid
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                # Check if an image is clicked
                x, y = event.pos
                for idx, rect in enumerate(image_rects):
                    if rect.collidepoint(x, y):
                        # Save the clicked image
                        img_src = profile_pics[idx]
                        save_image(img_src, idx)
                        print(f"Image {idx+1} saved.")

        screen.fill((0, 0, 0))  # Fill the screen with black
        for i, image in enumerate(loaded_images):
            x = (i % 4) * (screen_width // 4)
            y = (i // 4) * (screen_height // 5)
            screen.blit(image, (x, y))

        pygame.display.flip()
        pygame.time.wait(100)

    pygame.quit()

# Function to save the selected image
def save_image(img_url, index):
    response = requests.get(img_url)
    if response.status_code == 200:
        with open(f"selected_image.jpg", 'wb') as f:
            f.write(response.content)

# Main function to run the program
def main():
    data_input = "23938161"  # Example input number
    name, address = get_name_and_address(data_input)
    print(f"Name: {name}")
    print(f"Address: {address}")

    if name != "Name not found":
        display_profile_pictures(name)
    else:
        print("No profile pictures found as the name was not found.")

if __name__ == "__main__":
    main()
