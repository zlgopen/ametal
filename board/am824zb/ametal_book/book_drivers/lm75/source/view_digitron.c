

int view_digitron_init (view_digitron_t *p_view_digitron, int digitron_id)
{
    if (p_view_digitron == NULL) {
        return -1;
    	}
    	view_init(&(p_view_digitron->isa), __view_update);
	p_view_digitron->digitron_id = digitron_id;
	return 0;
}
