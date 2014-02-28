top    = '.'
out    = 'build'
js_lib = 'libs/minified-web.js'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def compile_coffee(ctx):
    coffee        = ctx.path.make_node('node_modules/.bin/coffee')
    pebble_coffee = ctx.path.make_node('src/js/pebble-integration.coffee')
    pebble_js     = ctx.path.get_bld().make_node('src/js/pebble-integration.js').bldpath()
    script_coffee = ctx.path.make_node('src/js/script.coffee')
    script_js     = ctx.path.get_bld().make_node('src/js/script.js')
    js_dir        = ctx.path.get_bld().make_node('src/js/')
    ctx(
        rule='%s --no-header -bco %s ${SRC} && echo "done brewing coffee"' % (coffee.bldpath(), js_dir.bldpath()),
        source=[pebble_coffee, script_coffee],
        target=[pebble_js,     script_js]
    )
    return 0


def build(ctx):
    ctx.load('pebble_sdk')
    ctx.add_pre_fun(compile_coffee)

    ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'), target='pebble-app.elf')

    # concats CSS and JS into the HTML file
    config_html   = ctx.path.make_node('src/js/index.html')
    css_code      = ctx.path.make_node('src/js/styles.css')
    js_code       = ctx.path.get_bld().make_node('src/js/script.js')
    complete_html = ctx.path.get_bld().make_node('src/js/complete.html')
    ctx(
        rule='(sed -e "/HERE BE CSS/r %s" -e "/HERE BE JS/r %s" ${SRC}) > ${TGT} && echo "done replace"' % (css_code.bldpath(), js_code.bldpath()),
        source=config_html,
        target=complete_html
    )

    # generate config.js from config.html by escaping every line and quotes
    config_js   = ctx.path.get_bld().make_node('src/js/index.html.js')
    ctx(
        rule='(echo window.config_html= && sed "s/\'/\\\\\\\'/g;s/^/\\\'/;s/$/\\\' +/" ${SRC} && echo "\'\';") > ${TGT} && echo "done concat"',
        source=complete_html,
        target=config_js
    )

    # make pebble-js-app.js by appending all JS files
    pebble_js = ctx.path.get_bld().make_node('src/js/pebble-integration.js').bldpath()
    js_files = [
        ctx.path.make_node('src/js/' + js_lib),
        config_js,
        pebble_js
    ]
    build_js = ctx.path.get_bld().make_node('src/js/pebble-js-app.js')
    ctx(rule='(cat ${SRC} > ${TGT})', source=js_files, target=build_js)

    # use build/src/js/pebble-js-app.js
    ctx.pbl_bundle(elf='pebble-app.elf', js=build_js)