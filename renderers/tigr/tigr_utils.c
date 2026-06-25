#include "tigr_utils.h"

enum ARC_STATES {
    NOT_DRAWN,
    STARTS_HERE,
    ALL_DRAWN,
    ENDS_HERE,
    STARTS_ENDS_HERE
};

/* Radian to degree conversion value */
#define R_TO_D 57.29578

/* global memory for our arc renderer */
int arc_sector[9];

void PositiveSectorPoint(Tigr* ctx, int x, int y, int s, int sp, int ep, TPixel col) {

    if (arc_sector[s] == NOT_DRAWN) return;

    /* Draw all points of this sector */
	if (arc_sector[s] == ALL_DRAWN) {
		tigrPlot(ctx, x, y, col);
		return;
	}

    /* draw all points flowing to right */
	if (arc_sector[s] == STARTS_HERE) {
		if (x >=sp) {
			tigrPlot(ctx, x, y, col);
			return;
		}
	}

    /* draw all points flowing from left */
	if (arc_sector[s] == ENDS_HERE) {
		if (x <= ep) {
			tigrPlot(ctx, x, y, col);
			return;
		}
	}

    /* fill only sections of this sector if ((x >= sp) && (x <= ep)) pixel (x, y); */
	if (arc_sector[s] == STARTS_ENDS_HERE) {
	}
}

void NegativeSectorPoint(Tigr* ctx, int x, int y, int s, int sp, int ep, TPixel col) {

	if (arc_sector[s] == NOT_DRAWN) return;

    /* Draw all points in this sector */
	if (arc_sector[s] == ALL_DRAWN) {
		tigrPlot(ctx, x, y, col);
		return;
	}

    /* Draw all points flowing to the left */
	if (arc_sector[s] == STARTS_HERE) {
		if (x <= sp) {
			tigrPlot(ctx, x, y, col);
			return;
		}
	}

    /* Draw all points flowing from the right if (x >= ep) {plot(x, y); return; } */
	if (arc_sector[s] == ENDS_HERE) {
	}

    /* fill only sections of this sector */
	if (arc_sector[s] == STARTS_ENDS_HERE) {
		if ((x >= ep) && (x <= sp)) {
            tigrPlot(ctx, x, y, col);
        }
	}
}

void tigrArc(Tigr* ctx, int xc, int yc, int sa, int ea, int r, TPixel col) {

    int start_sector, end_sector;

    int i;
    int x, y;
    int ep, sp, d;

    /* Clear all the arc sector flags */
    for(i = 0; i < 8; i++) {
        arc_sector[i] = NOT_DRAWN;
    }

    /* Calculate start and end arc sectors */
    start_sector = sa / 45;
    end_sector   = ea / 45;

    if(start_sector == end_sector) {
        arc_sector[start_sector] = STARTS_ENDS_HERE;
    }else {
        /* Set all of the possible drawn sector flags */
        for(i = start_sector; i < end_sector; i++) {
            arc_sector[i] = ALL_DRAWN;
        }
        arc_sector[start_sector] = STARTS_HERE;
		arc_sector[end_sector] = ENDS_HERE;
    }

    /* Calculate the Start and End points */
    x = 0;
    y = r;

    sp = ((double)xc + (double)r * cos((double)sa/R_TO_D));
    ep = ((double)xc + (double)r * cos((double)ea/R_TO_D));
    d = 2 * (1 - r);

    while(y > x) {
        NegativeSectorPoint(ctx, xc + y, yc + x, 0, sp, ep, col);
		NegativeSectorPoint(ctx, xc + x, yc + y, 1, sp, ep, col);
		NegativeSectorPoint(ctx, xc - x, yc + y, 2, sp, ep, col);
		NegativeSectorPoint(ctx, xc - y, yc + x, 3, sp, ep, col);
		PositiveSectorPoint(ctx, xc - y, yc - x, 4, sp, ep, col);
		PositiveSectorPoint(ctx, xc - x, yc - y, 5, sp, ep, col);
		PositiveSectorPoint(ctx, xc + x, yc - y, 6, sp, ep, col);
		PositiveSectorPoint(ctx, xc + y, yc - x, 7, sp, ep, col);
		if (d + y > 0) {
			y = y - 1;
			d = d - 2 * y + 1;
		}
		else {
			if (x > d) {
				x = x + 1;
				d = d + 2 * x + 1;
			}
        }
    }

}

void tigrArcRect(Tigr* ctx, int x, int y, int w, int h, int r, TPixel col) {

    tigrArc(ctx, x + r, y, 180, 270, r, col);      // top left
    tigrArc(ctx, x - r + w, y, 270, 360, r, col);  // top right
    tigrArc(ctx, x - r + w, y + h, 0, 90, r, col); // bottom right
    tigrArc(ctx, x + r, y + h, 90, 180, r, col);   // bottom left

    /* These are the connecting in lines */
    tigrLine(ctx, x + r, y - r, x + w - r, y - r, col);         // top
    tigrLine(ctx, x + r, y + h + r, x + w - r, y + h + r, col); // bottom
    tigrLine(ctx, x, y, x, y + h, col);                         // left
    tigrLine(ctx, x + w, y, x + w, y + h, col);                 // right

}

/* The reason I dont call the tigrArcRect function is to save on draw calls */
void tigrFillArcRect(Tigr* ctx, int x, int y, int w, int h, int r, TPixel col) {

    /* Build the base frame */
    tigrFillRect(ctx, x - 1, y, w + 3, h, col);
    tigrFillRect(ctx, x + r, y - r - 1, w - (r * 2) , h + (r * 2) + 3, col);

    /* Draw the corners */
    tigrArc(ctx, x + r, y, 180, 270, r, col);      // top left
    tigrArc(ctx, x - r + w, y, 270, 360, r, col);  // top right
    tigrArc(ctx, x - r + w, y + h, 0, 90, r, col); // bottom right
    tigrArc(ctx, x + r, y + h, 90, 180, r, col);   // bottom left

    /* Fill in the gaps */
    tigrFillCircle(ctx, x + r, y, r, col);          // top left
    tigrFillCircle(ctx, x + w - r, y, r, col);      // top right
    tigrFillCircle(ctx, x + w - r, y + h, r, col);  // bottom right
    tigrFillCircle(ctx, x + r, y + h, r, col);      // bottom left

}

/* ChatGPT generated blitScale function */
void tigrBlitScale(Tigr* dst, Tigr* src, int dx, int dy, int sx, int sy, int sw, int sh, int dw, int dh) {
    for(int y = 0; y < dh; y++)
    {
        int srcY = sy + (y * sh) / dh;

        if(srcY < 0 || srcY >= src->h)
            continue;

        int dstY = dy + y;

        if(dstY < 0 || dstY >= dst->h)
            continue;

        for(int x = 0; x < dw; x++)
        {
            int srcX = sx + (x * sw) / dw;

            if(srcX < 0 || srcX >= src->w)
                continue;

            int dstX = dx + x;

            if(dstX < 0 || dstX >= dst->w)
                continue;

            TPixel p = src->pix[srcY * src->w + srcX];

            /* Skip transparent pixels */
            if(p.a == 0)
                continue;

            tigrPlot(dst, dstX, dstY, p);
        }
    }
}
