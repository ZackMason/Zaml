# Zaml
A simple file format similar to Yaml

---

A basic Zaml config file looks something like this

```
---
# Comments must start with a '#'
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
		
# Any of these work as bools
settings: ~
	fullscreen: false
	mute: True
	vsync: off
	uncappedFPS: On
	
tests: true
	-: on
		title: test1
		file: test1.zaml
	-: off
		title: bench_test
		file: bench_test.zaml
  ```
