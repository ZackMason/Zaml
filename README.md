# Zaml
A simple file format similar to Yaml

---

A basic Zaml config file looks something like this

```
---
version: 0.1.1
window: ~
	width: 640
	height: 480
	title: Zaml is great
	font: ~
		file: Fonts/NotoMono-Regular.ttf
		size: 12

credits: ~
	-: ~
		name: Bill Gates
		role: Programmer
	-: ~
		name: Pablo Picasso
		role: Artist

settings: ~
	fullscreen: false
	mute: false
	
tests: true
	-: on
		title: test1
		file: test1.zaml
	-: off
		title: bench_test
		file: bench_test.zaml
  ```
