top    = '.'
out    = 'build'
js_lib = 'libs/minified-web.js'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    ctx.load('pebble_sdk')

    ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'), target='pebble-app.elf')

    pebble_integration = ctx.path.get_bld().make_node('src/js/pebble-js-app.js')
    ctx(rule='../web_compile', target=pebble_integration)

    # use build/src/js/pebble-js-app.js
    ctx.pbl_bundle(elf='pebble-app.elf', js=pebble_integration)