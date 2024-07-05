import Reconciler from 'react-reconciler'
import diffProperties from "./diffProperties";

//import {
    //NativeContainer,
    //createNativeInstance,
    //appendNativeElement,
    //updateNativeElement,
    //removeNativeElement,
//  getHostContextNode
//} from './native-adapter.js'

// Toggle to checkout reconciler call sequences
const SHOW_RECONCILER_CALLS = true
const log = (...args) => SHOW_RECONCILER_CALLS && console.log(...args)



const hostConfig = {
  // Methods for first-time rendering
  // --------------------------------
    
    appendChildToContainer (parent, stateNode) {
	log("appendChildToContainer");	
	parent.children.push(stateNode);
    },


    appendChild (parent, stateNode) {
	log("appendChild");	
	parent.children.push(stateNode);
    },


    
    appendInitialChild (parent, stateNode) {
	if(stateNode && stateNode.props)
	    log('appendInitialChild', parent.props.name , "<--", stateNode.props.name)
	else
	    log('appendInitialChild', parent.props.name , "<--", stateNode);
	parent.children.push(stateNode);

	parent.update(parent.props)
    },
    
    
    
    createInstance (type, props, internalInstanceHandle) {
	log('createInstance', type)
	return new NativeElement(type, props)
    },


    
    createTextInstance (text, rootContainerInstance, internalInstanceHandle) {
	log('createTextInstance', text)
	//return new NativeElement("text", {name:"text", text:text});
	return text;
    },

    
    finalizeInitialChildren (wordElement, type, props) {
	log('finalizeInitialChildren')
	return false
    },

    
    getPublicInstance (instance) {
	log('getPublicInstance')
	return instance
    },

    
    now: Date.now,

    prepareForCommit () {
	log('prepareForCommit')
    },

    
    prepareUpdate (wordElement, type, oldProps, newProps) {
	log('prepareUpdate')
	wordElement.update(newProps);
	
	//return diffProperties(type, oldProps, newProps);
	return true;
    },


    resetAfterCommit () {
	log('resetAfterCommit')
    },
    
    
    resetTextContent (wordElement) {
	log('resetTextContent')
    },

    
    getRootHostContext (instance) {
	log('getRootHostContext')
	return getHostContextNode(instance)
    },

    
    getChildHostContext (instance) {
	log('getChildHostContext')
	return {}
    },
    
    shouldSetTextContent (type, props) {
	log('shouldSetTextContent')
	return false
    },

    
    
    // Methods for updating state
    // --------------------------
    commitTextUpdate (textInstance, oldText, newText) {
	log('commitTextUpdate')
    },
    
    commitUpdate (
	instance, updatePayload, type, oldProps, newProps, finishedWork
    ) {
	log('commitUpdate ICI',)
	instance.props = newProps;
    },
    

    removeChild(parent, stateNode){
	log('removeChild IS IT THE ONE ?');
	const index = parent.children.indexOf(stateNode);
	parent.children.splice(index, 1);
	//stateNode.delete(stateNode);
    },

    
    removeChildFromContainer (parent, stateNode) {
	log('removeChildFromContainer')
	const index = parent.children.indexOf(stateNode);
	parent.children.splice(index, 1);
	//stateNode.delete(stateNode);
    },


    insertBefore(parent, child, beforeChild) {
	log('insertBefore')
	const index = parent.children.indexOf(beforeChild);
	parent.children.splice(index, 0, child);
	//console.log("found", index);
    },
    
    
    useSyncScheduling: true,
    supportsMutation: true
}


let RootNodeInstance

const getHostContextNode = rootNode => {
  if (rootNode) RootNodeInstance = rootNode
  else {
    console.warn(`${rootNode} is not an valid root instance.`)
      RootNodeInstance = new NativeElement("root", {name:"root"}); //NativeContainer()
  }
  return RootNodeInstance
}


// Singleton
const reconciler = Reconciler(hostConfig)
const root = new NativeElement("root", {name:"root"}); //NativeContainer()
const container = reconciler.createContainer(root, false)

export const Text = 'TEXT'
export const Pixel = 'PIXEL'
export const Button = "BUTTON"
export const View = "VIEW"

export const ImGuiRenderer = {
  render (reactElement) {
      reconciler.updateContainer(reactElement, container);
      return root;
  }
}
