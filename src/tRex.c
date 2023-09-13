#include "../include/tRex.h"

#include <math.h>
#include <stdlib.h>


/**
 * @ Animation states.
 */
enum status {
    CRASHED,
    DUCKING,
    JUMPING,
    RUNNING,
    WAITING,
};

Trex* Trex_constructor()
{
    Trex* this = malloc(sizeof(Trex));
    // this->spritePos = spritePos;
    this->xPos = 0;
    this->yPos = 0;
    this->xInitialPos = 0;
    
    // Position when on the ground.
    this->groundYPos = 0;
    this->currentFrame = 0;
    // this->currentAnimFrames = [];
    this->blinkDelay = 0;
    this->blinkCount = 0;
    this->animStartTime = 0;
    this->timer = 0;
    this->msPerFrame = 1000 / FPS;

    // this->config = config;
    this->DROP_VELOCITY = -5;
    this->FLASH_OFF = 175;
    this->FLASH_ON = 100;
    this->HEIGHT = 47;
    this->HEIGHT_DUCK = 25;
    this->INTRO_DURATION = 1500;
    this->SPEED_DROP_COEFFICIENT = 3;
    this->SPRITE_WIDTH = 262;
    this->START_X_POS = 50;
    this->WIDTH = 44;
    this->WIDTH_DUCK = 59;

    // this->jumpConfig = normalJumpConfig;
    this->GRAVITY = 0.25;
    this->MAX_JUMP_HEIGHT = 50;
    this->MIN_JUMP_HEIGHT = 45;
    this->INITIAL_JUMP_VELOCITY = -20;

    // Current status.
    // this->status = Trex.status.WAITING;
    this->jumping = false;
    this->ducking = false;
    this->jumpVelocity = 0;
    this->reachedMinHeight = false;
    this->speedDrop = false;
    this->jumpCount = 0;
    this->jumpspotX = 0;
    this->altGameModeEnabled = false;
    this->flashing = false;

    init(this);
    return this;
}

void Trex_destructor(Trex* this)
{
    free(this);
}

void init(Trex* this)
{
    // this->groundYPos = Runner.defaultDimensions.HEIGHT - this->HEIGHT - Runner.config.BOTTOM_PAD;
    this->yPos = this->groundYPos;
    this->minJumpHeight = this->groundYPos - this->MIN_JUMP_HEIGHT;

    draw(this, 0, 0);
    update(this, 0, WAITING);
}

/**
 * Assign the appropriate jump parameters based on the game speed.
 */
void enableSlowConfig(Trex* this)
{
    // const jumpConfig = Runner.slowDown ? Trex.slowJumpConfig : Trex.normalJumpConfig;
    // Trex.config = Object.assign(Trex.config, jumpConfig);

    // this->adjustAltGameConfigForSlowSpeed();
}

/**
 * Setter whether dino is flashing.
 */
void setFlashing(Trex* this, bool status)
{
    this->flashing = status;
}

/**
 * Setter for the jump velocity.
 * The approriate drop velocity is also set.
 * @param {number} setting
 */
void setJumpVelocity(Trex* this, int setting)
{
    this->INITIAL_JUMP_VELOCITY = -setting;
    this->DROP_VELOCITY = -setting / 2;
}

/**
 * Set the animation status.
 * @param {!number} deltaTime
 * @param {Trex.status=} opt_status Optional status to switch to.
 */

void update(Trex* this, int deltaTime, int opt_status)
{
    this->timer += deltaTime;

    // Update the status.
    if (opt_status) {
        this->status = opt_status;
        this->currentFrame = 0;
        // this->msPerFrame = Trex.animFrames[opt_status].msPerFrame;
        // this->currentAnimFrames = Trex.animFrames[opt_status].frames;

        if (opt_status == WAITING) {
            // this->animStartTime = getTimeStamp();
            setBlinkDelay(this);
        }
    }
    // Game intro animation, T-rex moves in from the left.
    if (this->playingIntro && this->xPos < this->START_X_POS) {
        this->xPos += round((this->START_X_POS / this->INTRO_DURATION) * deltaTime);
        this->xPos += 0;
        this->xInitialPos = this->xPos;
    }

    if (this->status == WAITING) {
        // blink(this, getTimeStamp());
    } else {
        // draw(this, this->currentAnimFrames[this->currentFrame], 0);
    }

    // Update the frame position.
    if (!this->flashing && this->timer >= this->msPerFrame) {
        // this->currentFrame = this->currentFrame == sizeof(this->currentAnimFrames) - 1 ? 0 : this->currentFrame + 1;
        this->timer = 0;
    }

    if (!this->altGameModeEnabled) {
        // Speed drop becomes duck if the down key is still being pressed.
        if (this->speedDrop && this->yPos == this->groundYPos) {
            this->speedDrop = false;
            setDuck(this, true);
        }
    }
}

/**
 * Draw the t-rex to a particular position.
 * @param {number} x
 * @param {number} y
 */
void draw(Trex* this, int x, int y)
{
    int sourceX = x;
    int sourceY = y;
    int sourceWidth = this->ducking && this->status != CRASHED ? this->WIDTH_DUCK : this->WIDTH;
    int sourceHeight = this->HEIGHT;
    const int outputHeight = sourceHeight;

    // int jumpOffset = Runner.spriteDefinition.TREX.JUMPING.xOffset;

    // Width of sprite changes on jump.
    if (this->altGameModeEnabled && this->jumping && this->status != CRASHED) {
        sourceWidth = this->WIDTH_JUMP;
    }

    // if (IS_HIDPI) {
    //     sourceX *= 2;
    //     sourceY *= 2;
    //     sourceWidth *= 2;
    //     sourceHeight *= 2;
    //     jumpOffset *= 2;
    // }

    // Adjustments for sprite sheet position.
    sourceX += this->spritePos.x;
    sourceY += this->spritePos.y;

    // Flashing.
    if (this->flashing) {
        if (this->timer < this->FLASH_ON) {
            // this->canvasCtx.globalAlpha = 0.5;
        } else if (this->timer > this->FLASH_OFF) {
            this->timer = 0;
        }
    }

    // Ducking.
    if (!this->altGameModeEnabled && this->ducking && this->status != CRASHED) {

        // this->canvasCtx.drawImage(Runner.imageSprite, sourceX, sourceY,
        //     sourceWidth, sourceHeight,
        //     this->xPos, this->yPos,
        //     this->WIDTH_DUCK, outputHeight);

    } else if (
        this->altGameModeEnabled && this->jumping && this->status != CRASHED) {
        // Jumping with adjustments.
        // this->canvasCtx.drawImage(
        //     Runner.imageSprite, sourceX, sourceY, sourceWidth, sourceHeight,
        //     this->xPos - jumpOffset, this->yPos, this->WIDTH_JUMP,
        //     outputHeight);
    } else {
        // Crashed whilst ducking. Trex is standing up so needs adjustment.
        if (this->ducking && this->status == CRASHED) {
            this->xPos++;
        }
        // Standing / running
        // this->canvasCtx.drawImage(Runner.imageSprite, sourceX, sourceY,
        //     sourceWidth, sourceHeight,
        //     this->xPos, this->yPos,
        //     this->WIDTH, outputHeight);
    }
    // this->canvasCtx.globalAlpha = 1;
}

/**
 * Sets a random time for the blink to happen.
 */
void setBlinkDelay(Trex* this)
{
    // this->blinkDelay = ceil(random() * this->BLINK_TIMING);
}

/**
 * Make t-rex blink at random intervals.
 * @param {number} time Current time in milliseconds.
 */
void blink(Trex* this, int time)
{
    int const deltaTime = time - this->animStartTime;

    if (deltaTime >= this->blinkDelay) {
        // draw(this, this->currentAnimFrames[this->currentFrame], 0);

        if (this->currentFrame == 1) {
            // Set new random delay to blink.
            setBlinkDelay(this);
            this->animStartTime = time;
            this->blinkCount++;
        }
    }
}

/**
 * Initialise a jump.
 * @param {number} speed
 */
void startJump(Trex* this, int speed)
{
    if (!this->jumping) {
        update(this, 0, JUMPING);
        // Tweak the jump velocity based on the speed.
        this->jumpVelocity = this->INITIAL_JUMP_VELOCITY - (speed / 10);
        this->jumping = true;
        this->reachedMinHeight = false;
        this->speedDrop = false;

        if (this->INVERT_JUMP) {
            this->minJumpHeight = this->groundYPos + this->MIN_JUMP_HEIGHT;
        }
    }
}

/**
 * Jump is complete, falling down.
 */
void endJump(Trex* this)
{
    if (this->reachedMinHeight && this->jumpVelocity < this->DROP_VELOCITY) {
        this->jumpVelocity = this->DROP_VELOCITY;
    }
}

/**
 * Update frame for a jump.
 * @param {number} deltaTime
 */
void updateJump(Trex* this, int deltaTime)
{
    // int const msPerFrame = this->animFrames[this->status].msPerFrame;
    const int msPerFrame = 1;
    int const framesElapsed = deltaTime / msPerFrame;

    // Speed drop makes Trex fall faster.
    if (this->speedDrop) {
        this->yPos += round(this->jumpVelocity * this->SPEED_DROP_COEFFICIENT * framesElapsed);
    } else if (this->INVERT_JUMP) {
        this->yPos -= round(this->jumpVelocity * framesElapsed);
    } else {
        this->yPos += round(this->jumpVelocity * framesElapsed);
    }

    this->jumpVelocity += this->GRAVITY * framesElapsed;

    // Minimum height has been reached.
    if (this->INVERT_JUMP && (this->yPos > this->minJumpHeight) || !this->INVERT_JUMP && (this->yPos < this->minJumpHeight) || this->speedDrop) {
        this->reachedMinHeight = true;
    }

    // Reached max height.
    if (this->INVERT_JUMP && (this->yPos > -this->MAX_JUMP_HEIGHT) || !this->INVERT_JUMP && (this->yPos < this->MAX_JUMP_HEIGHT) || this->speedDrop) {
        endJump(this);
    }

    // Back down at ground level. Jump completed.
    if ((this->INVERT_JUMP && this->yPos) < this->groundYPos || (!this->INVERT_JUMP && this->yPos) > this->groundYPos) {
        reset(this);
        this->jumpCount++;

        // if (Runner.audioCues) {
        //     Runner.generatedSoundFx.loopFootSteps();
        // }
    }
}

/**
 * Set the speed drop. Immediately cancels the current jump.
 */
void setSpeedDrop(Trex* this)
{
    this->speedDrop = true;
    this->jumpVelocity = 1;
}

/**
 * @param {boolean} isDucking
 */
void setDuck(Trex* this, bool isDucking)
{
    if (isDucking && this->status != DUCKING) {
        update(this, 0, DUCKING);
        this->ducking = true;
    } else if (this->status == DUCKING) {
        update(this, 0, RUNNING);
        this->ducking = false;
    }
}

/**
 * Reset the t-rex to running at start of game.
 */
void reset(Trex* this)
{
    this->xPos = this->xInitialPos;
    this->yPos = this->groundYPos;
    this->jumpVelocity = 0;
    this->jumping = false;
    this->ducking = false;
    update(this, 0, RUNNING);
    this->midair = false;
    this->speedDrop = false;
    this->jumpCount = 0;
}
