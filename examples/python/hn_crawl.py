# -*- coding: utf-8 -*-

import urllib.parse

import requests
import lxml.etree

next_url = 'http://news.ycombinator.com'
total_posts = 0

while next_url is not None:

    response = requests.get(next_url)

    document = lxml.etree.HTML(response.text)

    for title in document.cssselect('tr.athing a.storylink'):
        print(title.text)
        total_posts += 1


    # ascertain the base url i.e. schema + domain
    urlinfo = urllib.parse.urlparse(next_url)
    base_url = urlinfo.scheme + '://' + urlinfo.netloc

    try:
        href = document.cssselect('a.morelink')[0].get('href')
    except:
        next_url = None
        continue

    next_url = urllib.parse.urljoin(base_url, href)

print('#' * 20)
print('total post: {}'.format(total_posts))
