public final class Line implements DrawableShape {

    ////////////////////////
    // Variables

    private final Point[] coord;
    private final int color;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public Line(final int color, final int[] x, final int[] y) {
        this.color = color;
        coord = new Point[2];
        coord[0] = new Point(x[0], y[0]);
        coord[1] = new Point(x[1], y[1]);
    }

    public Line(final int x1, final int y1, final int x2, final int y2, final int color) {
        this.color = color;
        coord = new Point[2];
        coord[0] = new Point(x1, y1);
        coord[1] = new Point(x2, y2);
    }

    public int getColor() {
        return color;
    }

    public Point[] getCoord() {
        return coord;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////

}
