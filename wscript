top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    ctx.load('pebble_sdk')

    ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'), target='pebble-app.elf')

    # concats CSS and JS into the HTML file
    config_html   = ctx.path.make_node('src/js/index.html')
    css_code      = ctx.path.make_node('src/js/styles.css')
    js_code       = ctx.path.make_node('src/js/script.js')
    complete_html = ctx.path.get_bld().make_node('src/js/complete.html')
    ctx(
        rule='(sed -e "/HERE BE CSS/r %s" -e "/HERE BE JS/r %s" ${SRC}) > ${TGT}' % (css_code.bldpath(), js_code.bldpath()),
        source=config_html,
        target=complete_html
    )

    # generate config.js from config.html by escaping every line and quotes
    config_js   = ctx.path.get_bld().make_node('src/js/index.html.js')
    ctx(rule='(echo config_html= && sed "s/\'/\\\\\\\'/g;s/^/\\\'/;s/$/\\\' +/" ${SRC} && echo "\'\';") > ${TGT}', source=complete_html, target=config_js)

    # make pebble-js-app.js by appending all JS files
    js_files = [
        ctx.path.make_node('src/js/zepto.min.js'),
        config_js,
        ctx.path.make_node('src/js/pebble-integration.js')
    ]
    build_js = ctx.path.get_bld().make_node('src/js/pebble-js-app.js')
    ctx(rule='(cat ${SRC} > ${TGT})', source=js_files, target=build_js)

    # use build/src/js/pebble-js-app.js
    ctx.pbl_bundle(elf='pebble-app.elf', js=build_js)
