
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//

/**
 * A function to create a linear key frame object.
 * @constructor
 * @param {number} time The time of the keyframe.
 * @param {number} value The value of the keyframe.
 */
FABRIC.RT.LinearKeyframe = function(time, value) {
  this.time = typeof time === 'number' ? time : 0;
  this.value = typeof value === 'number' ? value : 0;
};

FABRIC.RT.LinearKeyframe.prototype = {
  // This method enables an animation evaluator to know what kind of data
  // track full of these data types should evaluate to.
  get valueType() {
    return 'Scalar';
  },
  getType: function() {
    return 'FABRIC.RT.LinearKeyframe';
  }
};

/**
 * overloaded constructor function for LinearKeyframe
 * @param {number} value The value of the keyframe.
 * @param {number} time The time of the keyframe.
 * @return {object} The linear key frame object.
 */
FABRIC.RT.linearKeyframe = function(value, time) {
  return new FABRIC.RT.LinearKeyframe(value, time);
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LinearKeyframe', {
    members: {
      time: 'Scalar', value: 'Scalar'
    },
    constructor: FABRIC.RT.LinearKeyframe,
    klBindings: {
      filename: 'LinearKeyframe.kl',
      sourceCode: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/LinearKeyframe.kl')
    }
  });
});


FABRIC.RT.LinearKeyframeTrack = function(name, color) {
  this.name = name ? name : "";
  this.color = color ? color : FABRIC.RT.rgb(1.0, 0.0, 0.0);
  this.keys = [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LinearKeyframeTrack', {
    members: {
      name: 'String',
      color: 'Color',
      keys: 'LinearKeyframe[]'
    },
    constructor: FABRIC.RT.LinearKeyframeTrack,
    kBindings: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/LinearKeyframe.kl')
  });
});


FABRIC.RT.LinearKeyframeTrackSet = function( name ) {
  this.name = name ? name : 'animationTrack';
  this.tracks = [];
};

FABRIC.appendOnCreateContextCallback(function(context) {
  context.RegisteredTypesManager.registerType('LinearKeyframeTrackSet', {
    members: {
      name: 'String',
      tracks: 'LinearKeyframeTrack[]'
    },
    constructor: FABRIC.RT.LinearKeyframeTrackSet,
    kBindings: FABRIC.loadResourceURL('FABRIC_ROOT/SceneGraph/RT/LinearKeyframe.kl')
  });
});


