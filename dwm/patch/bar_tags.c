#define BAR_OFFSET 4

int
width_tags(Bar *bar, BarArg *a)
{
	int w, i, tw;

	for (w = 0, i = 0; i < NUMTAGS; i++) {
		tw = TEXTW(tagicon(bar->mon, i));
		if (tw > lrpad)
			w += tw;
	}
	return w + BAR_OFFSET;
}

int
draw_tags(Bar *bar, BarArg *a)
{
	int invert;
	int w, x = a->x;
	unsigned int i, occ = 0, urg = 0;
	char *icon;
	Client *c;
	Monitor *m = bar->mon;

	for (c = m->clients; c; c = c->next) {
		occ |= c->tags;
		if (c->isurgent)
			urg |= c->tags;
	}
	for (i = 0; i < NUMTAGS; i++) {

		icon = tagicon(bar->mon, i);
		invert = 0;
		w = TEXTW(icon);
		if (w <= lrpad)
			continue;
		drw_setscheme(drw, scheme[
			m->tagset[m->seltags] & 1 << i
			? SchemeTagsSel
			: urg & 1 << i
			? SchemeUrg
			: SchemeTagsNorm
		]);
		if (i == 0)
			drw_text(drw, x, a->y, w, a->h, lrpad / 2, "", invert, False);
		drw_text(drw, x + BAR_OFFSET, a->y, w, a->h, lrpad / 2, icon, invert, False);
		drawindicator(m, NULL, occ, x, a->y, w, a->h, i, -1, invert, tagindicatortype);
		if (ulineall || m->tagset[m->seltags] & 1 << i)
			drw_rect(drw, x + ulinepad + BAR_OFFSET, bh - ulinestroke - ulinevoffset, w - (ulinepad * 2), ulinestroke, 1, 0);
		x += w;
	}

	return 1;
}

int
click_tags(Bar *bar, Arg *arg, BarArg *a)
{
	int i = 0, x = lrpad / 2;

	do {
		x += TEXTW(tagicon(bar->mon, i));
	} while (a->x >= x && ++i < NUMTAGS);
	if (i < NUMTAGS) {
		arg->ui = 1 << i;
	}
	return ClkTagBar;
}
