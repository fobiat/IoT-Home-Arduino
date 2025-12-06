#!/usr/bin/env python3
"""Basic markdown link and anchor checker for tracked markdown files.
Checks relative file links and anchors, and does best-effort HTTP GET for external links.
"""
import re, sys, urllib.request
from pathlib import Path
import subprocess

root=Path(__file__).resolve().parents[1]
# get tracked md files
p=subprocess.run(['git','-C',str(root),'ls-files','*.md'], capture_output=True, text=True)
files=[root/line for line in p.stdout.splitlines() if line.strip()]
if not files:
    print('No markdown files found')
    sys.exit(0)
link_re=re.compile(r"\[([^\]]+)\]\(([^)]+)\)")
http_ok=lambda code: code<400
failures=[]

def slugify(s):
    s=s.strip().lower()
    s=re.sub(r'`+','',s)
    s=re.sub(r"[^a-z0-9 -]",'',s)
    s=re.sub(r"\s+",'-',s)
    s=re.sub(r'-+','-',s)
    return s

for f in files:
    text=f.read_text(encoding='utf-8')
    print(f"\nChecking: {f.relative_to(root)}")
    for m in link_re.finditer(text):
        url=m.group(2).strip()
        if url.startswith('http://') or url.startswith('https://'):
            try:
                req=urllib.request.Request(url, headers={'User-Agent':'link-checker/1.0'})
                with urllib.request.urlopen(req, timeout=10) as resp:
                    code=resp.getcode()
                    if not http_ok(code):
                        failures.append((f, url, f'HTTP {code}'))
                        print('  FAIL external', url, '->', code)
                    else:
                        print('  ok external', url)
            except Exception as e:
                failures.append((f, url, str(e)))
                print('  FAIL external', url, '->', e)
        elif url.startswith('#'):
            target_anchor=url[1:]
            headings=[h for h in re.findall(r'^(#+)\s*(.*)', text, flags=re.M)]
            slugs=[slugify(h[1]) for h in headings]
            if slugify(target_anchor) not in slugs:
                failures.append((f, url, 'missing-anchor'))
                print('  FAIL anchor', url)
            else:
                print('  ok anchor', url)
        else:
            if '#' in url:
                rel,anchor=url.split('#',1)
            else:
                rel,anchor=url, None
            target=(f.parent/rel).resolve()
            if not target.exists():
                failures.append((f, url, 'missing-file'))
                print('  FAIL file', url, '-> not found', 'resolved:', target)
            else:
                print('  ok file', url)
                if anchor:
                    txt=target.read_text(encoding='utf-8')
                    headings=[h for h in re.findall(r'^(#+)\s*(.*)', txt, flags=re.M)]
                    slugs=[slugify(h[1]) for h in headings]
                    if slugify(anchor) not in slugs:
                        failures.append((f, url, 'missing-anchor-in-target'))
                        print('    FAIL anchor in target', anchor)
                    else:
                        print('    ok anchor in target', anchor)

print('\nSummary:')
if failures:
    print('Failures found:')
    for f,url,reason in failures:
        print('-', f.relative_to(root), url, reason)
    sys.exit(2)
else:
    print('No broken links detected (basic check).')
    sys.exit(0)
