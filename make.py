# -*- coding: utf-8 -*-

import re
import os

class Config:
    src = 'src/IR101.md'
    dest = 'IR101.md'

def import_resource(path):
    if os.path.isfile(path):
        return '\n' + open(path).read() + '\n'
    else:
        return ''

def main():
    raw = open(Config.src).read()
    build = re.sub(Config.pattern, import_resource, raw)
    open(Config.dest, 'w').write(build)


if __name__ == '__main__':
    main()
