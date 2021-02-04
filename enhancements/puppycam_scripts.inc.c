static void newcam_angle_rotate(struct newcam_hardpos *params)
{
    newcam_yaw += 0x100;
    print_text(32,32,"hi i am a script");
    print_text_fmt_int(32,48,"%d",newcam_yaw);
}




/**
 * C-Up First-Person Cam
 *  - Can now look 360 degrees around Mario
 *  - Can return to any arbitrary camera mode upon press of C-Down
 */


// return function; set this to whatever function you want
static void newcam_open_cam(struct newcam_hardpos *params);
static void (*newcam_c_up_cam_return)(struct newcam_hardpos *params) = newcam_open_cam;

// The speed of changing Mario's look angle with the analog stick
#define NEWCAM_C_UP_SPEED 6

#define ABS(x) ((x < 0) ? -(x) : (x))
#define DISTANCE(x, y) (ABS(x - y))
static void newcam_c_up_cam(struct newcam_hardpos *params) {
    newcam_distance_target = 175;

    newcam_yaw  += (s32) gPlayer1Controller->stickX * NEWCAM_C_UP_SPEED;
    newcam_tilt += (s32) gPlayer1Controller->stickY * NEWCAM_C_UP_SPEED;

    if (gPlayer1Controller->buttonPressed & D_CBUTTONS) {
        params->newcam_hard_script = newcam_c_up_cam_return;
        newcam_tilt = 1500;
        newcam_yaw = -gMarioState->faceAngle[1]-0x4000;
        set_mario_action(gMarioState, ACT_IDLE, 0);
    }

    // vanilla feature: access wing cap stage
    if (gMarioState->floor->type == SURFACE_LOOK_UP_WARP
        && save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1) >= 10) {
        if (DISTANCE(newcam_yaw, 16384) < 50 && DISTANCE(newcam_tilt, -10000) < 50) {
            params->newcam_hard_script = newcam_c_up_cam_return;
            newcam_tilt = 1500;
            newcam_yaw = -gMarioState->faceAngle[1]-0x4000;
            level_trigger_warp(gMarioState, WARP_OP_UNKNOWN_01);
        }
    }
}


/**
 * Open Area Cam
 *  - 8-direction camera with C-up mode and zoom-out functionality
 *  - Comes with every feature from Aglab's Parallel Cam
 *  - Amount of directions can be customized
 */

static int newcam_open_cam_zoomed_out = 0;
// Change the number of angles here
#define NUM_ANGLES 8
#define _OPEN_CAM_DELTA (0x10000 / NUM_ANGLES)

static void newcam_open_cam(struct newcam_hardpos *params) {
    if (!newcam_open_cam_zoomed_out) newcam_distance_target = 1000;

    // Look at Mario
    params->newcam_hard_lookX = (s16)(gMarioState->pos[0]);
    params->newcam_hard_lookY = (s16)(gMarioState->pos[1] + 125.0f);
    params->newcam_hard_lookZ = (s16)(gMarioState->pos[2]);

    // The main 8-direction control
    if (gPlayer1Controller->buttonPressed & R_CBUTTONS)
        newcam_yaw -= _OPEN_CAM_DELTA;
    if (gPlayer1Controller->buttonPressed & L_CBUTTONS)
        newcam_yaw += _OPEN_CAM_DELTA;


    // Entry for C-Up mode
    if (gPlayer1Controller->buttonPressed & U_CBUTTONS) {
        if (newcam_open_cam_zoomed_out == 1) {
            newcam_open_cam_zoomed_out = 0;
        }
        else if (gMarioState->action & ACT_FLAG_ALLOW_FIRST_PERSON) {
            newcam_yaw = -gMarioState->faceAngle[1]-0x4000;
            params->newcam_hard_script = newcam_c_up_cam;
            set_mario_action(gMarioState, ACT_WAITING_FOR_DIALOG, 0);
        }
    }

    // Super-zoom out
    if (gPlayer1Controller->buttonPressed & D_CBUTTONS) {
        newcam_distance_target += 400;
        newcam_open_cam_zoomed_out = 1;
    }


    // Parallel Cam Feature: Center behind Mario
    if (gPlayer1Controller->buttonPressed & U_JPAD) {
        newcam_yaw = -gMarioState->faceAngle[1]-0x4000;
    }

    // Parallel Cam Feature: Reset the axes of the camera
    if (gPlayer1Controller->buttonPressed & D_JPAD) {
        newcam_yaw = newcam_yaw % _OPEN_CAM_DELTA >= 0x1000 ?
                     newcam_yaw + _OPEN_CAM_DELTA - (newcam_yaw % _OPEN_CAM_DELTA) :
                     newcam_yaw - (newcam_yaw % _OPEN_CAM_DELTA);
    }

    // Parallel Cam Feature: fine tune the camera angle
    // with the D-pad
    if (gPlayer1Controller->buttonDown & R_JPAD)
        newcam_yaw += 100;
    if (gPlayer1Controller->buttonDown & L_JPAD)
        newcam_yaw -= 100;
}


/**
 * Cylinder Cam
 * - Rotates around the center of the bounding box but still follows Mario
 * - Can potentially be adapted into a Sphere Cam
 * - Ideal Mode: NC_MODE_FIXED_NOMOVE
 */

#define MIDPOINT(x, y) ((x + y) / 2)

static void vec3_copy_from_s16_vec(Vec3f dst, s16 *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

static void s16_vec_copy_from_vec3(s16 *dst, Vec3f src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

extern void vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32  dist, s16  pitch, s16  yaw);
extern void vec3f_get_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, s16 *pitch, s16 *yaw);

static void newcam_cylinder_cam(struct newcam_hardpos *params) {
    params->newcam_hard_lookX = (s16) MIDPOINT(params->newcam_hard_X1, params->newcam_hard_X2);
    params->newcam_hard_lookY = (s16) gMarioState->pos[1];
    params->newcam_hard_lookZ = (s16) MIDPOINT(params->newcam_hard_Z1, params->newcam_hard_Z2);
    

    Vec3f cam_look;
    vec3_copy_from_s16_vec(cam_look, &params->newcam_hard_lookX);

    f32 dist;
    s16 pitch, yaw;

    vec3f_get_dist_and_angle(gMarioState->pos, cam_look, &dist, &pitch, &yaw);

    Vec3f result;
    vec3f_set_dist_and_angle(cam_look, result, dist + 1500.0f, pitch, yaw + 0x8000);

    s16_vec_copy_from_vec3(&params->newcam_hard_camX, result);
    params->newcam_hard_camY += 500;
}

