import webpack from 'webpack'
import path from 'path'
import CopyWebpackPlugin from 'copy-webpack-plugin'
import HTMLWebpackPlugin from 'html-webpack-plugin'
import { getIfUtils } from 'webpack-config-utils'

const config = (env, ifProduction, ifNotProduction) => ({
  entry: {
    main: [
      './src/index.jsx'
    ]
  },
  output: {
		filename: "[name].bundle.js",
		path: path.resolve(__dirname, 'dist'),
		publicPath: ''
	},

	mode: ifProduction('production', 'development'),
	devtool: 'eval',
	devServer: {
		port: 8080,
		historyApiFallback: true
	},
	
	resolve: {
		alias: {
			reason: path.resolve(__dirname, "lib/js/src"),
			src: path.resolve(__dirname, "src"),
			assets: path.resolve(__dirname, "assets")
		},
		extensions: [".js", ".jsx", ".json"]
	},
	
	module: {
		rules: [
			{
				exclude: /node_modules/, // don't transpile node_modules
				test: /\.jsx$/,          // do transpile any files ending in .jsx
				use: {
					loader: 'babel-loader',
					options: {
						plugins: ['@babel/plugin-transform-react-jsx']
					}
				}
			},
			{ test: /\.css$/, loaders: ['style-loader', 'css-loader']},
			{ test: /\.(woff|woff2|eot|ttf|svg|png)$/, loader: 'file-loader?name=assets/[name].[ext]'}
		]
	},
	plugins: [
		new webpack.DefinePlugin({
			'process.env.NODE_ENV': JSON.stringify(env.production ? 'production' : 'development'),
			ENVIRONMENT: JSON.stringify({
				isDev: !env.production,
				shouldHotReloadEpics: env.hotEpics !== undefined
			})
		}),
		
		new HTMLWebpackPlugin({
			//hash: true,
			excludeChunks: [],
			template: './base.html'
		}),
		
		new webpack.NamedModulesPlugin(),

		new CopyWebpackPlugin([
			// amr codecs, lazy-loaded for arm playback used in gt-card-files
			{ from: './assets/favicon', to: './' },
			// copy version file to deployment root
			
		]),

	]
})

const envextract = config => env => {
	const { ifProduction, ifNotProduction } = getIfUtils(env)
	return config(env, ifProduction, ifNotProduction)
}

export default envextract(config)