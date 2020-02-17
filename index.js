const goButton = document.getElementById('go-button')
goButton.addEventListener('click', () => {
  const reply = window.sendSync('go')
  const message = `Result: ${reply}`
  document.getElementById('go-button-result').innerHTML = message
})
