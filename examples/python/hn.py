# -*- coding: utf-8 -*-

import sys
import time
import types
import pprint
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

        urlinfo = urllib.parse.urlparse(response.url)
        base_url = urlinfo.scheme + '://' + urlinfo.netloc

        document = lxml.etree.HTML(response.text)

        # hacker news uses table rows for it's interface.
        # since there's no clear isolation of data elements,
        # we're going to have create our own.
        headings = document.cssselect('table.itemlist td.title a.storylink')
        subtext = document.cssselect('table.itemlist td.subtext')

        for head, opt in zip(headings, subtext):

            item = {}
            item['name'] = head.text
            item['url'] = head.get('href')

            link_to_comments = opt.cssselect('span.age a')[0].get('href')
            link_to_comments = urllib.parse.urljoin(base_url, link_to_comments)
            item['comments'] = link_to_comments

            # sometimes posts without a user creep in
            try:
                item['user'] = opt.cssselect('a.hnuser')[0].text
            except IndexError:
                item['user'] = '(N/A)'
            yield item
            

        try:
            href = document.cssselect('a.morelink')[0].get('href')
        except:
            return

        next_url = urllib.parse.urljoin(base_url, href)

        crawler.schedule_request(next_url)

class PrintComponent:

    def process(self, payload, crawler):
        pprint.pprint(payload)
        return payload

class CrawlTimeComponent:

    def process(self, payload, crawler):
        payload['collected_at'] = time.ctime()
        return payload

if __name__ == "__main__":
    crawler = Crawler(HackerNewsSpider)
    crawler.add_pipeline_component(CrawlTimeComponent)
    crawler.add_pipeline_component(PrintComponent)
    crawler.run()
