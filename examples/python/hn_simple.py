# -*- coding: utf-8 -*-

'''
    fetch the title of the latest posts on page 1 of hacker news

    because who doesn't like hackers news?
'''

import requests
import lxml.etree

URL = 'http://news.ycombinator.com'

# fetch the page
response = requests.get(URL)

# parse the page
document = lxml.etree.HTML(response.text)

# on inspecting the HTML of the document, you'll see that every HN
# post has an anchor tag with the class 'story' inside a table row
# with a class 'athing'. We use the cssselect library in order to
# select all those elements that match this description. The result is a 
# list of lxml.etree.Element objects.
#
# you can also use xpath to select elements, for example:
# document.xpath("//tr[@class='athing']//a[@class='storylink']"):
# selects the same elements as the css expression 'tr.athing a.storylink'

for title in document.cssselect('tr.athing a.storylink'):
    print(title.text)
