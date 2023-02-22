const capture = () => {
    const captureElement = document.getElementById('tableWords')
    html2canvas(captureElement)
        .then(canvas => {
            canvas.style.display = 'none'
            document.body.appendChild(canvas)
            return canvas
        })
        .then(canvas => {
            const image = canvas.toDataURL('image/png').replace('image/png', 'image/octet-stream')
            const a = document.createElement('a')
            a.setAttribute('download', 'words-table.png')
            a.setAttribute('href', image)
            a.click()
            canvas.remove()
        })
}

window.onload = () => {
    setTimeout(() => { // this timeout is for making sure that all html elements are loaded
        const btnSaveTable = document.getElementById('btnSaveTable')
        btnSaveTable.addEventListener('click', capture)

    }, 1000);
}
