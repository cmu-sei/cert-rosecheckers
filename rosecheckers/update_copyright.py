#!/usr/bin/env python

# <legal>CERT Rosecheckers# 
# Copyright 2022 Carnegie Mellon University.# 
# NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.# 
# Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.# 
# [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.# 
# CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.# 
# This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.# 
# DM21-0505</legal># 

import fnmatch
import json
import io
import os
import re
import subprocess
import sys
import tarfile
import urllib

VERSION_MAGIC_STR = '{{BASP_VERSION}}'

COPYRIGHT_BLACKLIST = [
    "./.git",
    "./.ccls-cache",
    "./.project",
    "./.cproject",
]
COPYRIGHT_FILENAME_MAP = {
    ".gitignore": r'# \1',
    "Vagrantfile": r'# \1',
    "Dockerfile": r'# \1',
    "README": r'\1',
    ".gitkeep": None,
}
COPYRIGHT_EXTENSION_MAP = {
    ".sh": r'# \1',
    ".py": r'# \1',
    ".properties": r'# \1',
    ".yml": r'# \1',

    ".java": r'// \1',
    ".c": r'// \1',
    ".h": r'// \1',
    ".C": r'// \1',
    ".H": r'// \1',
    ".js":   r'// \1',

    ".erb": r'<!-- \1 -->',
    ".md":  r'<!-- \1 -->',

    ".sql": r'-- \1',

    # for HTML footer
    ".template": r'<p>\1',

    # These file extensions can be ignored
    ".json": None,
    ".jpg": None,
    ".png": None,
    ".gif": None,
    ".css": None,
    ".pdf": None,
    ".lua": None,
    ".docx": None,
    ".xlsx": None,
    ".pptx": None,
    ".xml": None,
    ".pyc": None,
    ".txt": None,
    ".csv": None,
    ".tsv": None,
    ".zip": None,
    ".org": None,
    ".xsl": None,
    ".prefs": None,
    ".class": None,
}

VERSION_FIX_FILES = ['license.txt']


def read_configuration(cfg_filename):
    '''
        Reads the ABOUT file, parses it as json, and returns the parsed object.
    '''
    with open(cfg_filename) as f:
        return json.load(f)


def update_copyright(cfg):
    '''
        Updating copyright info in each file that contains it
    '''
    warnings = args.warnings
    new_legal = "<legal>" + "\n".join(cfg['legal']) + "</legal>"
    for dirpath, _, files in os.walk('.'):
        if any([dirpath.startswith(x) for x in COPYRIGHT_BLACKLIST]):
            continue

        for filename in files:
            full_path = os.path.join(dirpath, filename)
            ext = os.path.splitext(full_path)[-1]
            regex = None
            if dirpath == "./bin":
                regex = COPYRIGHT_EXTENSION_MAP[".sh"]
            elif ext in COPYRIGHT_EXTENSION_MAP:
                regex = COPYRIGHT_EXTENSION_MAP[ext]
            elif filename in COPYRIGHT_FILENAME_MAP:
                regex = COPYRIGHT_FILENAME_MAP[filename]
            else:
                if warnings:
                    print("WARNING: Not checking copyright in " + full_path)

            if not regex:
                continue

            prot_new_legal = re.sub(r'(?m)(.*)', regex, new_legal)
            with io.open(full_path, 'r+', encoding="utf-8") as fp:
                contents = fp.read()
                # This matches all lines where the <legal> tags
                # live. So anything on these lines outside the <legal>
                # tags will get erased. IOW this presumes that the
                # lines with <legal> tags have nothing else of
                # importance.
                match = re.search(r'(?im)^.*?<legal>(.|\n)*?</legal>.*?$',
                                  contents)
                if not match:
                    if warnings:
                        print("WARNING: No copyright for " + full_path)
                    continue
                if not warnings:
                    new_contents = (contents[:match.start(0)] +
                                    prot_new_legal +
                                    contents[match.end(0):])
                    fp.seek(0)
                    fp.write(new_contents)
                    fp.truncate()

    return 'DONE'


def adjust_version_numbers(cfg):
    '''
        Replacing version numbers with the version from the ABOUT file.
    '''
    for filename in VERSION_FIX_FILES:
        with io.open(filename, 'r', encoding="utf-8") as input_f:
            new_str = input_f.read().replace(VERSION_MAGIC_STR, cfg['version'])
        with io.open(filename, 'w', encoding="utf-8") as output_f:
            output_f.write(new_str)
    return 'DONE'


if __name__ == '__main__':
    import argparse
    p = argparse.ArgumentParser(description="Script to update copyright info")
    p.add_argument('-w', '--warnings', action='store_true',
                   help="Print warnings, but don't update legal info in source files")
    args = p.parse_args()

    cfg = read_configuration('ABOUT')

    # Setup the local directory for packaging
    update_copyright(cfg)
    adjust_version_numbers(cfg)
