# -*- coding: utf-8 -*-

import sys
import types
import urllib.parse

import requests
import lxml.etree

class Crawler:

    def __init__(self, spider, **kwargs):
        self._spider = spider()
        self._pipeline = []
        self._next_url = [spider.start_url]


    def add_pipeline_component(self, component):
        self._pipeline.append(component())

    def schedule_request(self, url):
        self._next_url.append(url)


    def _invoke_spider(self, payload):
        # tbd = to be determined
        tbd = self._spider.parse(payload, self)
        if isinstance(tbd, types.GeneratorType):
            for chunk in tbd:
                self._pipe(chunk)
        else:
            self._pipe(tbd)


    def _pipe(self, payload):
        for component in self._pipeline:
            payload = component.process(payload, self)

    def run(self):
        
        while self._next_url:

            next_url = self._next_url.pop()
            request = requests.get(next_url)

            if request.status_code >= 200 and request.status_code < 300:
                payload = self._invoke_spider(request)
            else:
                errstr = 'Crawler Error: {} {}({})'.format(
                    request.status_code,
                    request.reason,
                    request.url
                )
                print(errstr, file=sys.stderr)
        else:
            print('Crawler has finished running')

class HackerNewsSpider:

    start_url = 'http://news.ycombinator.com'

    def parse(self, response, crawler):

        document = lxml.etree.HTML(response.text)
        for title in document.cssselect('tr.athing a.storylink'):
            yield title.text

        urlinfo = urllib.parse.urlparse(response.url)
        base_url = urlinfo.scheme + '://' + urlinfo.netloc

        try:
            href = document.cssselect('a.morelink')[0].get('href')
        except:
            return

        next_url = urllib.parse.urljoin(base_url, href)

        crawler.schedule_request(next_url)

class PrintComponent:

    def process(self, payload, crawler):
        print(payload)
        return payload

if __name__ == "__main__":
    crawler = Crawler(HackerNewsSpider)
    crawler.add_pipeline_component(PrintComponent)
    crawler.run()
