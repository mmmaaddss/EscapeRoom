from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options

options = Options()
options.headless = True
service = Service(ChromeDriverManager().install())
driver = webdriver.Chrome(service=service, options=options)

url = 'https://www.facebook.com/public/kasper.thorup'
driver.get(url)

driver.implicitly_wait(10)
images = driver.find_elements(By.CSS_SELECTOR, "img._1glk._6phc.img") # The typical class for images might be something like '_1glk _6phc img', but you need to verify this

profile_pics = [img.get_attribute('src') for img in images[:10]] # Collect the src of the first 10 profile images
for i, src in enumerate(profile_pics, 1):
    print(f"Profile Picture {i}: {src}")

driver.quit()
