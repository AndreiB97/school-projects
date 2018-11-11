public final class Triangle implements DrawableShape {

    ////////////////////////
    // Variables

    private final Point[] coord;
    private final int outlineColor;
    private final int fillColor;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public int getFillColor() {
        return fillColor;
    }

    public int getOutlineColor() {
        return outlineColor;
    }

    public Point[] getCoord() {
        return coord;
    }

    public Triangle(final int[] x, final int[] y, final int outline, final int fill) {
        coord = new Point[3];
        for (int i = 0; i < 3; i++) {
            coord[i] = new Point(x[i], y[i]);
        }
        outlineColor = outline;
        fillColor = fill;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////

}
