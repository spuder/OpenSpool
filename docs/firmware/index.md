---
layout: default
title: Firmware Directory
nav_exclude: true
search_exclude: true
---

{% assign files = site.static_files | where_exp: "file", "file.path contains '/firmware/'" %}

## Available Files

<table>
<tr>
    <th>Name</th>
    <th>Last Modified</th>
    <th>Size</th>
</tr>
{% for file in files %}
<tr>
    <td><a href="{{ file.path | relative_url }}">{{ file.name }}</a></td>
    <td>{{ file.modified_time | date: "%Y-%m-%d %H:%M" }}</td>
    <td class="size">{{ file.size | divided_by: 1024 }}KB</td>
</tr>
{% endfor %}
</table>