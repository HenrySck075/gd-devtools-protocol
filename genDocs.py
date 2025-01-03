from __future__ import annotations

head = """GD DevTools Protocol Domains Reference (INCOMPLETE)
---

Except from some derivations (such as `Browser` domain renamed to `Game`), this api is similar to the [Chrome DevTools Protocol](https://chromedevtools.github.io/devtools-protocol/) so if you're familiar with it then there shouldn't be any issues working with this :D

This .md file is generated from protocols.json so it's guaranteed to match with the protocols list returned.

> **Note** that this reference ==only documents what's currently available in the mod itself==, so don't expect every features of CDP available (especially on the early releases of the mod)
> 
> **Note 2** most of this docs will sound similar to the docs from the aforementioned protocol and it's intended. Don't ask anything about that.


<style type="text/css">
  tbody>tr>td:first-child {text-align: right}
</style>

"""

import json
from typing import Required, TypedDict, NotRequired

file = json.load(open("resources/protocols.json"))

class ObjectArrayType(TypedDict, total=False):
    id: Required[str]
    description: str
    optional: bool 

_ObjectProps = TypedDict("_ObjectProps", {
    "type": str,
    "$ref": str,
    "enums": list[str],
    # seems to be so
}, total=False)

class ObjectProps(_ObjectProps, ObjectArrayType, total=False):
    items: list[ObjectArrayType]

class Event(TypedDict, total=False):
    name: Required[str]
    description: Required[str]
    params: list[ObjectProps]
class Command(Event, total=False):
    returnValue: list[ObjectProps]

class Domain(TypedDict,total=False):
    domain: Required[str]
    description: str
    types: list[dict]
    commands: list[Command]
    events: list[Event]

primitiveTypes = ['integer', 'number', 'boolean', 'string', 'object',
                  'any', 'array', 'binary']
def genPropTable(p: ObjectProps):
    t = ""
    primitive = True
    if "type" in p:
        t = p['type']
    else:
        t = p.get("$ref", "")
        primitive = False
    return f"""
  <tr>
    <td><code>{p['id']}</code>{"<br>(optional)" if p.get('optional', False) else ""}</td>
    <td><strong>{t if primitive else f'<a href="#{d.lower() if "." not in t else ""}{t.lower()}">{t}</a>'}</strong>{"<br>"+p['description'] if "description" in p else ""}{f"<br>Allowed Values: <code>{', '.join(p.get("enums",[]))}</code>" if t == "string" else ""}</td>
  </tr>"""

def genPropsTable(d: str, cmd: Event):
    doc=f"""### {d}.`{cmd['name']}`
{cmd.get("description","")}

"""
    params = cmd.get("params")
    ret = cmd.get("returnValue")
    if (params != None or ret != None):
        doc+="<table>"
        if (params!=None):
            doc+="""
<thead>
<tr>
<th colspan="2">Parameters</th>
</tr>
</thead>
<tbody>"""
            for p in params:
                doc+=genPropTable(p)
            doc+="\n</tbody>\n"
        if (ret!=None):
            doc+="""
<thead>
<tr>
<th colspan="2">Return Values</th>
</tr>
</thead>
<tbody>"""
            for r in ret:
                doc+=genPropTable(r)
            doc+="\n</tbody>\n"
        doc+="</table>\n\n"
    return doc


domains: list[Domain] = file["domains"]
for i in domains:
    d = i['domain']
    doc = f"""# `{d}` Domain
{i.get('description','')}

"""
    
    commands = i.get("commands",[])
    if len(commands) != 0:
        doc+="## Methods\n"
        for cmd in commands:
            doc+=genPropsTable(d, cmd)
        head+=doc+"\n"
    events = i.get("events",[])
    if len(events) != 0:
        doc+="## Events\n"
        for evt in events:
            doc+=genPropsTable(d, evt)
        head+=doc+"\n"

open("o.md","w").write(head)
