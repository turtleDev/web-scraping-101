# -*- coding: utf-8 -*-

import re
import os

class Config:
    src = 'src/IR101.md'
    dest = 'IR101.md'
    pattern = '{{import\((.+)\)}}'

def import_resource(match):

    if not match:
        return ''

    path = match.groups()[0]

    if os.path.isfile(path):
        return open(path).read()
    else:
        return ''

def main():
    raw = open(Config.src).read()
    build = re.sub(Config.pattern, import_resource, raw)
    open(Config.dest, 'w').write(build)


if __name__ == '__main__':
    main()
