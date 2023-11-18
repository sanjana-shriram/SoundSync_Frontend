// gsap.from('.page', {duration: 1, x: '25%'} )

// gsap.from('.cursor', {duration: 2, x: '-10000%', ease: 'Linear.easeNone',} )
const deviceWidth = window.innerWidth;

// Assuming this.cursor is your cursor element and deviceWidth is the width of your device
document.addEventListener("DOMContentLoaded", function() {
    // Your GSAP animation code here

    const cursor = document.querySelector('.cursor');
    // const travelRange = 20;
    this.tl = new TimelineLite({
      onComplete: function() {
        // this.delay(2).restart(true);
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
  


// CSSPlugin.defaultTransformPerspective = 800;

// var tl = new TimelineMax({repeat:2, yoyo:false});
// tl.to(".page", 4, {rotationY:-180, transformOrigin:"50% 0%", ease:Linear.easeNone})
//   ///hide the front page at -90 degrees
//   .set(".page .front", {autoAlpha:0}, "-=2")