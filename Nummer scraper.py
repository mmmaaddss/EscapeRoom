import requests
from bs4 import BeautifulSoup

Datainput = "93996237" #tlf nummer, navn eller adresse

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
    "what": Datainput,  # Use the constant here
    "where": ""
}

payload = ""
headers = {
    "cookie": "JSESSIONID=14B9EA7776D343469E88B5C69277734B; citrix_ns_id=AAQ7V_4UZjt-OhwBAAAAADt0dZwUe2YPITMaOxp2RHuaSbeplDOyleVCWhvr3oAzOw%3D%3D3gEVZg%3D%3DhP9ddcaL2ict4cikCj_s5ck5dM0%3D",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "Accept-Language": "da,en-US;q=0.9,en;q=0.8,da-DK;q=0.7",
    "Connection": "keep-alive",
    "Cookie": "CookieInformationConsent=%7B%22website_uuid%22%3A%22a1a27bd1-3105-437d-8726-217c1ce6c6a0%22%2C%22timestamp%22%3A%222024-04-09T08%3A23%3A55.324Z%22%2C%22consent_url%22%3A%22https%3A%2F%2F118.dk%2F%22%2C%22consent_website%22%3A%22118.dk%22%2C%22consent_domain%22%3A%22118.dk%22%2C%22user_uid%22%3A%22f7b1d0f7-b569-4d9a-b4ca-c41a63daee79%22%2C%22consents_approved%22%3A%5B%22cookie_cat_necessary%22%5D%2C%22consents_denied%22%3A%5B%22cookie_cat_functional%22%2C%22cookie_cat_statistic%22%2C%22cookie_cat_marketing%22%2C%22cookie_cat_unclassified%22%5D%2C%22user_agent%22%3A%22Mozilla%2F5.0%20%28Macintosh%3B%20Intel%20Mac%20OS%20X%2010_15_7%29%20AppleWebKit%2F537.36%20%28KHTML%2C%20like%20Gecko%29%20Chrome%2F121.0.0.0%20Safari%2F537.36%22%7D; JSESSIONID=CA86EB1E288AE22FED5ED3AA2D895239; citrix_ns_id=AAE7HP0UZjtbzosCAAAAADt0dZwUe2YPITMaO2WJNjJE6IncAALYISii4Mi4bRtlOw==oQAVZg==Gq4gstEOOImLbzpjeIia3D0wRDs=",
    "DNT": "1",
    "Referer": "https://118.dk/search/go?pageSize=25&whatName=&whatCategory=&whatPhone=&whatOther=&whereStreet=&whereHouseNumber=&whereZip=&whereCity=&category=&keyword=&what=93996237&where=",
    "Sec-Fetch-Dest": "document",
    "Sec-Fetch-Mode": "navigate",
    "Sec-Fetch-Site": "same-origin",
    "Sec-Fetch-User": "?1",
    "Upgrade-Insecure-Requests": "1",
    "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36",
    "sec-ch-ua": "\"Not A(Brand\";v=\"99\", \"Google Chrome\";v=\"121\", \"Chromium\";v=\"121\"",
    "sec-ch-ua-mobile": "?0",
    "sec-ch-ua-platform": "\"macOS\""
}

response = requests.request("GET", url, data=payload, headers=headers, params=querystring)
#print(response.text)
html_content = response.text
soup = BeautifulSoup(html_content, 'html.parser') #Parse the HTML

result_block = soup.find('div', class_='result-block')
name = result_block.find('h3').text.strip() if result_block.find('h3') else "Name not found" #river navn ned
address = result_block.find('address').text.strip() if result_block.find('address') else "Address not found" #futter fat i adressen

print(f"Name: {name}")
print(f"Address: {address}")
