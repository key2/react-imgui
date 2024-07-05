import './polyfill.js'
import React from 'react'
import { ImGuiRenderer} from './renderer.js'
const { Component, Fragment } = React


class App extends Component {
  constructor () {
    super()
      this.state = {hello: '', textpress: ''}
  }
    
    render () {
	const { hello, textpress, value } = this.state
	//const {value, setvalue} = this.useState("blabla");
	
	return (
	    <Fragment>
		<view>
		    <sameline>
			<vslider size={{width: 45, height:300}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			<vslider size={{width: 45, height:300}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			<vslider size={{width: 45, height:300}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			<vslider size={{width: 45, height:300}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			<vslider size={{width: 45, height:300}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			<group>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			</group>
			<group>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			</group>
			<group>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			</group>
			<group>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			    <vslider size={{width: 45, height:148}} bounds={{min:0.0, max:1234.4}} onChange={(val) => console.log(val)}/>
			</group>
		    </sameline>
		    <input value="1234" onChangeText={(txt) => console.log(txt)}/>
		    <input onChangeText={(txt) => console.log(txt)}/>
		    <input onChangeText={(txt) => console.log(txt)}/>

		    <button title="Press me to create 2 extra buttons"
			    onPress={() => this.setState({hello: <button onPress={() => this.setState({hello:''})}    />})}
			    size={{width: 300, height:30}}
			    style={{color:"#ff1123", color_hovered:'red', color_active:'green'}}
		    />
		    
		    {hello}
		    <text style={{color:"#ff1123", background_color:'red'}}>
			This is a text: Hello World !!!
		    </text>
		    <button title="Press me"
			    style={{color:"#ff0312"}}
			    onPress={() => {console.log('Simple Button pressed'); this.setState({textpress:('')}) } } />
		    <view >
			{hello}
			<text style={{color:"green"}}>
			    This is another text !!!
			</text>
			{textpress}
			<button title="Press me"
				onPress={() => this.setState({textpress:( <text>You pressed me!</text>)})}
			/>
		    </view>
		    <text>{JSON.stringify({a:2, b:5})}</text>
		    {hello}
		</view>
		<vslider style={{color_bg:'blue'}}/>
	    </Fragment>	
	)
    }
}




globalThis.myval = ImGuiRenderer.render(<App/>);


