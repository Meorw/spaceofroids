var camera, scene, renderer

function init() {
	camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 10000)
	
	scene = new THREE.Scene()
	
	renderer = new THREE.WebGLRenderer()
	renderer.setSize(window.innerWidth, window.innerHeight)

	document.body.appendChild(renderer.domElement)
}

function update() {
	requestAnimationFrame(update)
	renderer.render(scene, camera)
}

init()
update()
