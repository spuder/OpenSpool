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
    <td class="size">
    {% assign file_path = file.path | prepend: site.source %}
    {% if file_path | file_exists %}
        {% assign file_size = file_path | file_size | divided_by: 1024.0 %}
        {{ file_size | round: 2 }} KB
    {% else %}
        N/A
    {% endif %}
    </td>
</tr>
{% endfor %}
</table>