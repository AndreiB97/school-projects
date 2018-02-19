public final class Polygon implements DrawableShape {

    ////////////////////////
    // Variables

    private final int pointNumber;
    private final Point[] coord;
    private final int outlineColor;
    private final int fillColor;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public Polygon(final int pointNumber, final int[] x, final int[] y,
                   final int outline, final int fill) {
        this.pointNumber = pointNumber;
        coord =  new Point[pointNumber];
        for (int i = 0; i < pointNumber; i++) {
            coord[i] = new Point(x[i], y[i]);
        }
        outlineColor = outline;
        fillColor = fill;
    }

    public int getFillColor() {
        return fillColor;
    }

    public int getOutlineColor() {
        return outlineColor;
    }

    public Point[] getCoord() {
        return coord;
    }

    public int getPointNumber() {
        return pointNumber;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////
}
