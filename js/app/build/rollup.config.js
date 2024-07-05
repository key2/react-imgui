const jsx = require('./jsx-plugin.js')
const commonjs = require('rollup-plugin-commonjs')
const nodeResolve = require('rollup-plugin-node-resolve')

module.exports = {
    input: 'src/index.js',
    output: {
	file: 'dist/app.js',
	format: 'esm'
    },
    external: ['os', 'renderer', 'NativeElement'],
    plugins: [
	jsx(),
	nodeResolve(),
	commonjs({ include: 'node_modules/**', sourceMap: false}),
	        
    ]
}
