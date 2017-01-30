# -*- coding: utf-8 -*-

import re
import os

class Config:
    src = 'src/WS101.md'
    dest = 'WS101.md'
    pattern = '{{import\((.+)\)}}'

def import_resource(match):

    if not match:
        return ''

    path = match.groups()[0]

    return ('# file: ' + path + '\n' + 
            '# ' +  ('-' * (6 + len(path))) + '\n\n' +
            open(path).read())

def main():
    raw = open(Config.src).read()
    build = re.sub(Config.pattern, import_resource, raw)
    open(Config.dest, 'w').write(build)

    # required for git pre-commit hook
    print(Config.dest)


if __name__ == '__main__':
    main()
