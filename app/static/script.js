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
  