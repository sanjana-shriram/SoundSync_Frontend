"use strict"

function getImages() {
  let xhr = new XMLHttpRequest()
  xhr.onreadystatechange = function() {
      if (this.readyState !== 4) return
      updatePage(xhr)
  }

  xhr.open("GET", "/app/get-global", true)
  xhr.send()
}

function updatePage(xhr) {
  if (xhr.status === 200) {
      let response = JSON.parse(xhr.responseText)
      updateList(response)
      return
  }

  if (xhr.status === 0) {
      displayError("Cannot connect to server")
      return
  }


  if (!xhr.getResponseHeader('content-type') === 'application/json') {
      displayError(`Received status = ${xhr.status}`)
      return
  }

  let response = JSON.parse(xhr.responseText)
  if (response.hasOwnProperty('error')) {
      displayError(response.error)
      return
  }

  displayError(response)
}

function updateList(items) {
}




gsap.from('.page', {duration: 1, x: '25%'} )

// gsap.from('.cursor', {duration: 2, x: '-10000%', ease: 'Linear.easeNone',} )

// Citation https://gsap.com/community/forums/topic/20383-consistent-speed-with-various-distance/


document.addEventListener("DOMContentLoaded", function() {
    const cursor = document.querySelector('.cursor');
    this.tl = new TimelineLite({
      onComplete: function() {
        this.restart(true);
      },
    });
    this.tl.fromTo(cursor, 6, {
      x: -100,
      ease: 'Linear.easeInOut',
    }, {
      x: 370,
      ease: 'Linear.easeInOut',
    });
    this.tl.play();
  });
  