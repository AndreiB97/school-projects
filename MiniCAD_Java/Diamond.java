public final class Diamond implements DrawableShape {

    ////////////////////////
    // Variables

    private final Point centerCoord;
    private final int verticalDiagonalLength;
    private final int horizontalDiagonalLength;
    private final int outlineColor;
    private final int fillColor;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public int getOutlineColor() {
        return outlineColor;
    }

    public int getFillColor() {
        return fillColor;
    }

    public Point getCenterCoord() {
        return centerCoord;
    }

    public int getHorizontalDiagonalLength() {
        return horizontalDiagonalLength;
    }

    public int getVerticalDiagonalLength() {
        return verticalDiagonalLength;
    }

    public Diamond(final int x, final int y, final int horizontal, final int vertical,
                   final int outline, final int fill) {
        centerCoord = new Point(x, y);
        horizontalDiagonalLength = horizontal;
        verticalDiagonalLength = vertical;
        outlineColor = outline;
        fillColor = fill;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////

}
