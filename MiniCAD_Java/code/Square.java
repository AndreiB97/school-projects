public final class Square implements DrawableShape {

    ////////////////////////
    // Variables

    private final Point upperLeftCoord;
    private final int length;
    private final int outlineColor;
    private final int fillColor;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public Point getUpperLeftCoord() {
        return upperLeftCoord;
    }

    public int getLength() {
        return length;
    }

    public int getOutlineColor() {
        return outlineColor;
    }

    public int getFillColor() {
        return fillColor;
    }

    public Square(final int x, final int y, final int length,
                  final int outlineColor, final int fillColor) {
        upperLeftCoord = new Point(x, y);
        this.length = length;
        this.outlineColor = outlineColor;
        this.fillColor = fillColor;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////

}
