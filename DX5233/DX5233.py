from lxml.etree import tostring
import lxml.html
import requests
from adremover import AdRemover
from bs4 import BeautifulSoup
# from bs4 import Comment

# def cleanMe(html):
#     soup = BeautifulSoup(html, "html5lib")    
#     [x.extract() for x in soup.find_all('script')]
#     [x.extract() for x in soup.find_all('meta')]
#     [x.extract() for x in soup.find_all('noscript')]
#     # [x.extract() for x in soup.find_all(text=lambda text:isinstance(text, Comment))]
#     return soup

url = 'https://soft98.ir'  # replace it with a url you want to apply the rules to  
rule_urls = ['ruadlist+easylist.txt', '1.txt']

rule_files = [url.rpartition('/')[-1] for url in rule_urls]





remover = AdRemover(*rule_files)

html = requests.get(url).text
document = lxml.html.document_fromstring(html)
remover.remove_ads(document)
# print("sdfsdf")
clean_html = tostring(document).decode("utf-8")
# print(cleanMe(clean_html))
soup=BeautifulSoup(clean_html)
# print(BeautifulSoup(clean_html))



with open('readme.html', 'w',encoding='utf-8') as f:
    f.write(str(soup))